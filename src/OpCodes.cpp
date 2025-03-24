/* DONE */
#include "OpCodes.hpp"
#include <string>

std::string OpCodeToString(OpCode code) {
    switch (code) {
        case OpCode::Nop:
            return "nop";
        case OpCode::Halt:
            return "halt";
        case OpCode::Add:
            return "add";
        case OpCode::Sub:
            return "sub";
        case OpCode::Mult:
            return "mult";
        case OpCode::Divide:
            return "divide";
        case OpCode::Mod:
            return "mod";
        case OpCode::Umi:
            return "umi";
        case OpCode::Power:
            return "power";
        case OpCode::Inc:
            return "inc";
        case OpCode::LocalInc:
            return "localInc";
        case OpCode::Dec:
            return "dec";
        case OpCode::LocalDec:
            return "localDec";
        case OpCode::Load:
            return "load";
        case OpCode::Store:
            return "store";
        case OpCode::LoadLocal:
            return "loadLocal";
        case OpCode::StoreLocal:
            return "storeLocal";
        case OpCode::And:
            return "and";
        case OpCode::Or:
            return "or";
        case OpCode::Not:
            return "not";
        case OpCode::Xor:
            return "xor";
        case OpCode::Pushi:
            return "pushi";
        case OpCode::Pushb:
            return "pushb";
        case OpCode::Pushd:
            return "pushd";
        case OpCode::Pushs:
            return "pushs";
        case OpCode::PushNone:
            return "pushNone";
        case OpCode::Pop:
            return "pop";
        case OpCode::IsEq:
            return "isEq";
        case OpCode::IsGt:
            return "isGt";
        case OpCode::IsGte:
            return "isGte";
        case OpCode::IsLt:
            return "isLt";
        case OpCode::IsLte:
            return "isLte";
        case OpCode::IsNotEq:
            return "isNotEq";
        case OpCode::Jmp:
            return "jmp";
        case OpCode::JmpIfTrue:
            return "jmpIfTrue";
        case OpCode::JmpIfFalse:
            return "jmpIfFalse";
            // case OpCodes::LvecIdx:
            //     return "lvecIdx";
            // case OpCodes::SvecIdx:
            //     return "svecIdx";
            // case OpCodes::LocalLvecIdx:
            //     return "locallvecIdx";
            // case OpCodes::LocalSvecIdx:
            return "localsvecIdx";
        case OpCode::Call:
            return "call";
        // case OpCodes::BuiltIn:
        //     return "builtin";
        case OpCode::Ret:
            return "ret";
            // case OpCodes::CreateList:
            //     return "createList";
            // case OpCodes::AppendList:
            //     return "appendList";
    }
    return "";
}