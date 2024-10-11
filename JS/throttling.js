function throttle(fnToThrottle, delay) {
    let execute = true;
    return function (...args) {
        if (execute) {
            fnToThrottle(...args);
            execute = false;
            setTimeout(() => {
                execute = true;
            }, delay);
        }
    };
}

let throttledFunction = throttle(onScroll, 2000);
