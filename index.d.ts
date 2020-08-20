declare class BloomFilterNative{
    constructor(errorRate: number, numberFiles: number);
    add(data: string):void;
    lookup(data: string):boolean;
}