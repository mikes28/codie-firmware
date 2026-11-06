use std::thread;
use std::time::Duration;
use codie::sim;
#[test]
fn dbg2() {
    sim::reset();
    let _t = thread::spawn(|| loop {
        thread::sleep(Duration::from_millis(1));
        codie::system::SysTick_Handler();
        sim::advance_timers(1);
    });
    let _f = thread::spawn(|| codie::boot::run());
    thread::sleep(Duration::from_millis(50));
    let _ = sim::usart_take_tx(codie::hal::addr::USART4);
    thread::sleep(Duration::from_millis(150));
    let _ = sim::usart_take_tx(codie::hal::addr::USART4);
    sim::usart_push_rx(codie::hal::addr::USART4, b"set hwiLed rgb 0 255 0 0\n");
    thread::sleep(Duration::from_millis(150));
    println!("OUT: {:?}", String::from_utf8_lossy(&sim::usart_take_tx(codie::hal::addr::USART4)));
}
