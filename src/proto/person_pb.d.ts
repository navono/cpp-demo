// package: tutorial
// file: person.proto

/* tslint:disable */
/* eslint-disable */

import * as jspb from "google-protobuf";

export class Person extends jspb.Message { 
    getName(): string;
    setName(value: string): Person;
    getId(): number;
    setId(value: number): Person;
    getEmail(): string;
    setEmail(value: string): Person;
    clearPhonesList(): void;
    getPhonesList(): Array<Person.PhoneNumber>;
    setPhonesList(value: Array<Person.PhoneNumber>): Person;
    addPhones(value?: Person.PhoneNumber, index?: number): Person.PhoneNumber;

    serializeBinary(): Uint8Array;
    toObject(includeInstance?: boolean): Person.AsObject;
    static toObject(includeInstance: boolean, msg: Person): Person.AsObject;
    static extensions: {[key: number]: jspb.ExtensionFieldInfo<jspb.Message>};
    static extensionsBinary: {[key: number]: jspb.ExtensionFieldBinaryInfo<jspb.Message>};
    static serializeBinaryToWriter(message: Person, writer: jspb.BinaryWriter): void;
    static deserializeBinary(bytes: Uint8Array): Person;
    static deserializeBinaryFromReader(message: Person, reader: jspb.BinaryReader): Person;
}

export namespace Person {
    export type AsObject = {
        name: string,
        id: number,
        email: string,
        phonesList: Array<Person.PhoneNumber.AsObject>,
    }


    export class PhoneNumber extends jspb.Message { 
        getNumber(): string;
        setNumber(value: string): PhoneNumber;
        getType(): Person.PhoneType;
        setType(value: Person.PhoneType): PhoneNumber;

        serializeBinary(): Uint8Array;
        toObject(includeInstance?: boolean): PhoneNumber.AsObject;
        static toObject(includeInstance: boolean, msg: PhoneNumber): PhoneNumber.AsObject;
        static extensions: {[key: number]: jspb.ExtensionFieldInfo<jspb.Message>};
        static extensionsBinary: {[key: number]: jspb.ExtensionFieldBinaryInfo<jspb.Message>};
        static serializeBinaryToWriter(message: PhoneNumber, writer: jspb.BinaryWriter): void;
        static deserializeBinary(bytes: Uint8Array): PhoneNumber;
        static deserializeBinaryFromReader(message: PhoneNumber, reader: jspb.BinaryReader): PhoneNumber;
    }

    export namespace PhoneNumber {
        export type AsObject = {
            number: string,
            type: Person.PhoneType,
        }
    }


    export enum PhoneType {
    MOBILE = 0,
    HOME = 1,
    WORK = 2,
    }

}

export class AddressBook extends jspb.Message { 
    clearPeopleList(): void;
    getPeopleList(): Array<Person>;
    setPeopleList(value: Array<Person>): AddressBook;
    addPeople(value?: Person, index?: number): Person;

    serializeBinary(): Uint8Array;
    toObject(includeInstance?: boolean): AddressBook.AsObject;
    static toObject(includeInstance: boolean, msg: AddressBook): AddressBook.AsObject;
    static extensions: {[key: number]: jspb.ExtensionFieldInfo<jspb.Message>};
    static extensionsBinary: {[key: number]: jspb.ExtensionFieldBinaryInfo<jspb.Message>};
    static serializeBinaryToWriter(message: AddressBook, writer: jspb.BinaryWriter): void;
    static deserializeBinary(bytes: Uint8Array): AddressBook;
    static deserializeBinaryFromReader(message: AddressBook, reader: jspb.BinaryReader): AddressBook;
}

export namespace AddressBook {
    export type AsObject = {
        peopleList: Array<Person.AsObject>,
    }
}
