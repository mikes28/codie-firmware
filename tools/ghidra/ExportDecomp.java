// ExportDecomp.java - Ghidra headless script
// Decompiles every function in the imported program and writes one C file per
// function plus a combined file, so the reconstruction can be diffed against it.
import ghidra.app.decompiler.*;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.*;
import ghidra.program.model.mem.MemoryBlock;
import ghidra.program.model.address.*;
import ghidra.util.task.TaskMonitor;
import java.io.*;
import java.util.*;

public class ExportDecomp extends GhidraScript {
    @Override
    public void run() throws Exception {
        String outDir = getScriptArgs().length > 0 ? getScriptArgs()[0] : "decomp";
        new File(outDir).mkdirs();

        DecompInterface di = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        opts.setEliminateUnreachable(true);
        opts.setCommentStyle(DecompileOptions.CommentStyleEnum.CPPStyle);
        di.setOptions(opts);
        di.toggleCCode(true);
        di.setSimplificationStyle("decompile");
        if (!di.openProgram(currentProgram)) {
            printerr("decompiler open failed");
            return;
        }
        FunctionManager fm = currentProgram.getFunctionManager();
        FunctionIterator it = fm.getFunctions(true);
        PrintWriter all = new PrintWriter(new FileWriter(outDir + "/ALL.c"));
        int n = 0, failed = 0;
        while (it.hasNext()) {
            if (monitor.isCancelled()) break;
            Function f = it.next();
            DecompileResults res = di.decompileFunction(f, 120, monitor);
            StringBuilder sb = new StringBuilder();
            sb.append("/* ==== ").append(f.getName()).append(" @ 0x")
              .append(Long.toHexString(f.getEntryPoint().getOffset())).append(" ==== */\n");
            if (res != null && res.decompileCompleted() && res.getDecompiledFunction() != null) {
                sb.append(res.getDecompiledFunction().getC());
            } else {
                failed++;
                sb.append("/* decompile failed: ").append(res == null ? "null" : res.getErrorMessage()).append(" */\n");
            }
            all.println(sb);
            all.flush();
            String fname = String.format("%s_0x%08X.c", f.getName(),
                    f.getEntryPoint().getOffset());
            try (PrintWriter w = new PrintWriter(new FileWriter(outDir + "/" + fname))) {
                w.println(sb);
            }
            n++;
        }
        all.close();

        // reference lists: strings and their xrefs
        try (PrintWriter w = new PrintWriter(new FileWriter(outDir + "/STRINGS.txt"))) {
            Listing l = currentProgram.getListing();
            DataIterator di2 = l.getDefinedData(true);
            while (di2.hasNext()) {
                Data d = di2.next();
                Object v = d.getValue();
                if (v instanceof String) {
                    w.println("0x" + Long.toHexString(d.getAddress().getOffset()) + "  " + v);
                }
            }
        }
        try (PrintWriter w = new PrintWriter(new FileWriter(outDir + "/FUNCS.txt"))) {
            FunctionIterator it2 = fm.getFunctions(true);
            while (it2.hasNext()) {
                Function f = it2.next();
                w.println("0x" + Long.toHexString(f.getEntryPoint().getOffset()) + "\t" +
                          f.getName() + "\tsize=" + f.getBody().getNumAddresses());
            }
        }
        try (PrintWriter w = new PrintWriter(new FileWriter(outDir + "/MEMORY.txt"))) {
            for (MemoryBlock b : currentProgram.getMemory().getBlocks()) {
                w.println(b.getName() + " 0x" + Long.toHexString(b.getStart().getOffset()) +
                          " - 0x" + Long.toHexString(b.getEnd().getOffset()) +
                          " r=" + b.isRead() + " w=" + b.isWrite() + " x=" + b.isExecute());
            }
        }
        println("ExportDecomp: functions=" + n + " failed=" + failed);
        di.dispose();
    }
}
