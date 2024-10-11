function debounce(fnToDebounce, delay) {
    let id = null;
    return function (...args) {
        clearTimeout(id);
        id = setTimeout(function () {
            fnToDebounce(...args);
        }, delay);
    };
}

let debouncedFunction = debounce(handleScroll, 2000);
