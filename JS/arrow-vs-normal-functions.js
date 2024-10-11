let global = {
    a: 10,
    b: function () {
        console.log(this.a);
        return () => {
            console.log(this.a);
        };
    },
};

let obj2 = {
    a: 10,
    b: () => {
        console.log(this.a);
    },
};

global.b()(); // 10 10
console.log(globalThis);
obj2.b(); // undefined
