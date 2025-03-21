#include "VM.hpp"

#include <cmath>
#include <stdexcept>
#include <algorithm>

#include "OpCodes.hpp"
#include "TModule.hpp"
#include "macros.hpp"
#include "ConstantTable.hpp"

void VM::error(const std::string &arg, const TMachineStackRecord &st1, const TMachineStackRecord &st2) {
    throw std::runtime_error(TStackRecordTypeToStr(st1.type()) + " and " + TStackRecordTypeToStr(st2.type()) +
                             " cannot be used with the " + arg + " operation");
}

void VM::runModule(std::shared_ptr<TModule> module) {
    module_ = module;
    run(module_->code());
}

void VM::run(const TProgram &code) {
    size_t ip = 0;  // instruction counter.

    while (true) {
        const auto &byteCode = code[ip];
        switch (byteCode.opCode) {
            case OpCodes::Nop:
                break;
            case OpCodes::Halt:
                break;
            case OpCodes::Add:
                addOp();
                break;
            case OpCodes::Sub:
                subOp();
                break;
            case OpCodes::Mult:
                multOp();
                break;
            case OpCodes::Divide:
                divOp();
                break;
            case OpCodes::Power:
                powerOp();
                break;
            case OpCodes::Umi:
                unaryMinusOp();
                break;
            case OpCodes::Inc:
                incOp(byteCode.index);
                break;
            case OpCodes::Dec:
                decOp(byteCode.index);
                break;
            case OpCodes::And:
                andOp();
                break;
            case OpCodes::Or:
                orOp();
                break;
            case OpCodes::Not:
                notOp();
                break;
            case OpCodes::Xor:
                xorOp();
                break;
            case OpCodes::IsLt:
                isLt();
                break;
            case OpCodes::IsGt:
                isGt();
                break;
            case OpCodes::IsGte:
                isGte();
                break;
            case OpCodes::IsLte:
                isLte();
                break;
            case OpCodes::Mod:
                modOp();
                break;
            case OpCodes::IsEq:
                isEq();
                break;
            case OpCodes::IsNotEq:
                isNotEq();
                break;
            case OpCodes::Pushi:
                push(byteCode.index);
                break;
            case OpCodes::Pushb:
                push(static_cast<bool>(byteCode.index));
                break;
            case OpCodes::Pushd:
                push(constantValueTable[byteCode.index].dvalue());
                break;
            case OpCodes::Pushs:
                push(constantValueTable[byteCode.index].svalue());
                break;
            case OpCodes::PushNone:
                push();
                break;
            // case OpCodes::Load:
            //     loadSymbol(byteCode.index);
            //     break;
            case OpCodes::Jmp:
                ip += byteCode.index - 1;
                break;
            case OpCodes::JmpIfTrue:
                if (stack_.pop().bvalue()) {
                    ip += byteCode.index - 1;
                }
                break;
            case OpCodes::JmpIfFalse:
                if (!stack_.pop().bvalue()) {
                    ip += byteCode.index - 1;
                }
                break;
            case OpCodes::Load:
                loadSymbol(byteCode.index);
                break;
        }
    }
}

void VM::store(int symTableIndex) {
    TMachineStackRecord record = stack_.pop();
    switch (record.type()) {
        case TStackRecordType::stInteger:
            // storeToSymbolTable(symTableIndex, std::get<int>(record.value()));
            break;
        case TStackRecordType::stBoolean:
            // storeToSymbolTable(symTableIndex, std::get<bool>(record.value()));
            break;
        case TStackRecordType::stDouble:
            // storeToSymbolTable(symTableIndex, std::get<double>(record.value()));
            break;
        case TStackRecordType::stString:
            // storeToSymbolTable(symTableIndex,
            // std::get<std::string>(record.value()));
            break;
        case TStackRecordType::stNone:
            break;
    }
}

void VM::load(int symTableIndex) {
    // auto symbol = getFromSymbolTable(symTableIndex);
    // switch (symbol.type())
    // {
    // case symInteger:
    //     stack_.push(symbol.ivalue());
    //     break;
    // case symDouble:
    //     stack_.push(symbol.dvalue());
    //     break;
    // case symBoolean:
    //     stack_.push(symbol.bvalue());
    //     break;
    // case symString:
    //     stack_.push(symbol.svalue());
    //     break;
    // }
}

void VM::addOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();

    auto st1_typ = st1.type();
    auto st2_typ = st2.type();

    if (st1_typ == TStackRecordType::stNone || st2_typ == TStackRecordType::stNone) {
        throw std::runtime_error("RunTimeError: Variable undefined");
    }

    if (st2_typ == TStackRecordType::stInteger) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st1.ivalue() + st2.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st1.dvalue() + st2.ivalue());
        } else {
            error("adding", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stBoolean) {
        error("adding", st2, st1);  // Can't add booleans
    } else if (st2_typ == TStackRecordType::stDouble) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st1.ivalue() + st2.dvalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st1.dvalue() + st2.dvalue());
        } else {
            error("adding", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stString) {
        if (st1_typ == TStackRecordType::stString) {
            stack_.push(st2.svalue() + st1.svalue());
            throw std::runtime_error(IMPLEMENTATION_MISSING);
        } else {
            error("adding", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stList) {
        if (st1_typ == TStackRecordType::stInteger) {
            auto list = st2.lvalue();
            list.emplace_back(st1.ivalue());
            stack_.push(list);
        } else if (st1_typ == TStackRecordType::stBoolean) {
            auto list = st2.lvalue();
            list.emplace_back(st1.bvalue());
            stack_.push(list);
        } else if (st1_typ == TStackRecordType::stDouble) {
            auto list = st2.lvalue();
            list.emplace_back(st1.dvalue());
            stack_.push(list);
        } else if (st1_typ == TStackRecordType::stString) {
            auto list = st2.lvalue();
            list.emplace_back(st1.svalue());
            stack_.push(list);
        } else if (st1_typ == TStackRecordType::stList) {
            auto trg = st2.lvalue();
            auto src = st1.lvalue();
            trg.insert(trg.end(), src.begin(), src.end());
            stack_.push(src);
        } else {
            error("adding", st2, st1);
        }
    } else {
        throw std::runtime_error("Internal Error: Unsupported datatype in add");
    }
}

void VM::multOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();

    auto st1_typ = st1.type();
    auto st2_typ = st2.type();

    if (st1_typ == TStackRecordType::stNone || st2_typ == TStackRecordType::stNone) {
        throw std::runtime_error("RunTimeError: Variable undefined");
    }

    if (st2_typ == TStackRecordType::stInteger) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st1.ivalue() * st2.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st1.dvalue() * st2.ivalue());
        } else if (st1_typ == TStackRecordType::stString) {
            auto ans = st1.svalue();
            for (int i = 2; i < st2.ivalue(); ++i) {
                ans += st1.svalue();
            }
            stack_.push(ans);
        } else if (st1_typ == TStackRecordType::stList) {
            auto buffer = st1.lvalue();
            auto newList = buffer;
            for (int i = 2; i < st2.ivalue(); ++i) {
                newList.insert(newList.end(), buffer.begin(), buffer.end());
            }
            stack_.push(newList);
        } else {
            error("multiplying", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stBoolean) {
        error("multiplying", st2, st1);  // Can't add booleans
    } else if (st2_typ == TStackRecordType::stDouble) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st1.ivalue() * st2.dvalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st1.dvalue() * st2.dvalue());
        } else {
            error("multiplying", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stString) {
        if (st1_typ == TStackRecordType::stString) {
            auto ans = st2.svalue();
            for (int i = 2; i < st1.ivalue(); ++i) {
                ans += st2.svalue();
            }
            stack_.push(ans);
        } else {
            error("multiplying", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stList) {
        if (st1_typ == TStackRecordType::stInteger) {
            auto buffer = st2.lvalue();
            auto newList = buffer;
            for (int i = 2; i < st1.ivalue(); ++i) {
                newList.insert(newList.end(), buffer.begin(), buffer.end());
            }
            stack_.push(newList);
        } else {
            throw std::runtime_error("Lists can only be multiplied by integers");
        }
    } else {
        throw std::runtime_error("Internal Error: Unsupported datatype in multiply");
    }
}

void VM::divOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();

    auto st1_typ = st1.type();
    auto st2_typ = st2.type();

    if (st1_typ == TStackRecordType::stNone || st2_typ == TStackRecordType::stNone) {
        throw std::runtime_error("RunTimeError: Variable undefined");
    }

    if (st2_typ == TStackRecordType::stInteger) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() / st1.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st2.ivalue() / st1.dvalue());
        } else {
            error("dividing", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stDouble) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st2.dvalue() / st1.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() / st1.dvalue());
        } else {
            error("dividing", st2, st1);
        }
    } else {
        throw std::runtime_error("Internal Error: Unsupported datatype in dividing");
    }
}

void VM::subOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();

    auto st1_typ = st1.type();
    auto st2_typ = st2.type();

    if (st1_typ == TStackRecordType::stNone || st2_typ == TStackRecordType::stNone) {
        throw std::runtime_error("RunTimeError: Variable undefined");
    }

    if (st2_typ == TStackRecordType::stInteger) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() - st1.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st2.ivalue() - st1.dvalue());
        } else {
            error("subtracting", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stDouble) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(st2.dvalue() - st1.ivalue());
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() - st1.dvalue());
        } else {
            error("subtracting", st2, st1);
        }
    } else {
        throw std::runtime_error(
            "Internal Error: Only integers and floats can be "
            "subtracted from each other");
    }
}

void VM::powerOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();

    auto st1_typ = st1.type();
    auto st2_typ = st2.type();

    if (st1_typ == TStackRecordType::stNone || st2_typ == TStackRecordType::stNone) {
        throw std::runtime_error("RunTimeError: Variable undefined");
    }

    if (st2_typ == TStackRecordType::stInteger) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(std::pow(st2.ivalue(), st1.ivalue()));
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(std::pow(st2.ivalue(), st1.dvalue()));
        } else {
            error("exponential", st2, st1);
        }
    } else if (st2_typ == TStackRecordType::stDouble) {
        if (st1_typ == TStackRecordType::stInteger) {
            stack_.push(std::pow(st2.dvalue(), st1.ivalue()));
        } else if (st1_typ == TStackRecordType::stDouble) {
            stack_.push(std::pow(st2.dvalue(), st1.dvalue()));
        } else {
            error("exponential", st2, st1);
        }
    } else {
        throw std::runtime_error("Internal Error: Data type not supported by power operator");
    }
}

void VM::unaryMinusOp() {
    auto st = stack_.pop();
    switch (st.type()) {
        case TStackRecordType::stInteger:
            stack_.push(-st.ivalue());
            break;
        case TStackRecordType::stDouble:
            stack_.push(-st.dvalue());
            break;
        default:
            throw std::runtime_error("Data type not supported by unary operator");
    }
}

void VM::incOp(int index) {
    auto &st = symboltable();
    auto symbol = st.get(index);
    switch (symbol.type()) {
        case TSymbolElementType::Integer:
            st.store(index, symbol.ivalue() + 1);
            break;
        case TSymbolElementType::Double:
            st.store(index, symbol.dvalue() + 1);
            break;
        default:
            throw std::runtime_error("Internal error: Illegal use of incBy on a non-integer/double type");
    }
}

void VM::decOp(int index) {
    auto &st = symboltable();
    auto symbol = st.get(index);
    switch (symbol.type()) {
        case TSymbolElementType::Integer:
            st.store(index, symbol.ivalue() - 1);
            break;
        case TSymbolElementType::Double:
            st.store(index, symbol.dvalue() - 1);
            break;
        default:
            throw std::runtime_error(
                "Internal error: Illegal use of deccBy on a "
                "non-integer/double type");
    }
}

void VM::andOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    if (st1.type() == TStackRecordType::stBoolean && st2.type() == TStackRecordType::stBoolean) {
        stack_.push(st1.bvalue() && st2.bvalue());
        return;
    }
    throw std::runtime_error("Incompatible types in AND operation");
}

void VM::orOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    if (st1.type() == TStackRecordType::stBoolean && st2.type() == TStackRecordType::stBoolean) {
        stack_.push(st1.bvalue() || st2.bvalue());
        return;
    }
    throw std::runtime_error("Incompatible types in OR operation");
}

void VM::notOp() {
    auto st = stack_.pop();
    if (st.type() == TStackRecordType::stBoolean) {
        stack_.push(!st.bvalue());
        return;
    }
    throw std::runtime_error("Incompatible type in NOT operation");
}

void VM::xorOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    if (st1.type() == TStackRecordType::stBoolean && st2.type() == TStackRecordType::stBoolean) {
        stack_.push(st1.bvalue() ^ st2.bvalue());
        return;
    }
    throw std::runtime_error("Incompatible types in XOR operation");
}

void VM::isLt() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();
    if (st1_type == TStackRecordType::stInteger) {
        if (st2.type() == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() < st1.ivalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() < st1.ivalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Less Than'' Operation");
        }
    } else if (st1_type == TStackRecordType::stDouble) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() < st1.dvalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() < st1.dvalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Less Than'' Operation");
        }
    } else {
        throw std::runtime_error("Incompatible types in ''Is Less Than'' Operation");
    }
}

void VM::isGt() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();
    if (st1_type == TStackRecordType::stInteger) {
        if (st2.type() == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() > st1.ivalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() > st1.ivalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater Than'' Operation");
        }
    } else if (st1_type == TStackRecordType::stDouble) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() > st1.dvalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() > st1.dvalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater Than'' Operation");
        }
    } else {
        throw std::runtime_error("Incompatible types in ''Is Greater Than'' Operation");
    }
}

void VM::isGte() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();
    if (st1_type == TStackRecordType::stInteger) {
        if (st2.type() == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() >= st1.ivalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() >= st1.ivalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
        }
    } else if (st1_type == TStackRecordType::stDouble) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() >= st1.dvalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() >= st1.dvalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
        }
    } else {
        throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
    }
}

void VM::isLte() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();
    if (st1_type == TStackRecordType::stInteger) {
        if (st2.type() == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() <= st1.ivalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() <= st1.ivalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
        }
    } else if (st1_type == TStackRecordType::stDouble) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() <= st1.dvalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(st2.dvalue() <= st1.dvalue());
        } else {
            throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
        }
    } else {
        throw std::runtime_error("Incompatible types in ''Is Greater-Equal Than'' Operation");
    }
}

void VM::modOp() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();

    if (st1_type == TStackRecordType::stNone || st2_type == TStackRecordType::stNone) {
        throw std::runtime_error("Variable undefined");
    }

    if (st2_type == TStackRecordType::stInteger) {
        if (st1_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() % st1.ivalue());
        } else if (st1_type == TStackRecordType::stDouble) {
            stack_.push(std::fmod(st2.ivalue(), st1.dvalue()));
        } else {
            throw std::runtime_error("Incompatible types in mod operation");
        }
    } else if (st2_type == TStackRecordType::stDouble) {
        if (st1_type == TStackRecordType::stInteger) {
            stack_.push(std::fmod(st2.dvalue(), st1.ivalue()));
        } else if (st1_type == TStackRecordType::stDouble) {
            stack_.push(std::fmod(st2.dvalue(), st1.dvalue()));
        } else {
            throw std::runtime_error("Incompatible types in mod operation");
        }
    } else {
        throw std::runtime_error("Incompatible types in mod operation");
    }
}

void VM::isEq() {
    auto st1 = stack_.pop();
    auto st2 = stack_.pop();
    auto st1_type = st1.type();
    auto st2_type = st2.type();

    if (st1_type == TStackRecordType::stBoolean && st2_type == TStackRecordType::stBoolean) {
        stack_.push(st1.bvalue() == st2.bvalue());
        return;
    }

    if (st1_type == TStackRecordType::stInteger) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(st2.ivalue() == st1.ivalue());
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(std::fabs(st1.ivalue() - st2.dvalue()) < 1e-9);
        } else {
            throw std::runtime_error("Incompatible types in equality test");
        }
    } else if (st1_type == TStackRecordType::stDouble) {
        if (st2_type == TStackRecordType::stInteger) {
            stack_.push(std::fabs(st1.ivalue() - st2.dvalue()) < 1e-9);
        } else if (st2_type == TStackRecordType::stDouble) {
            stack_.push(std::fabs(st1.dvalue() - st2.dvalue()) < 1e-9);
        } else {
            throw std::runtime_error("Incompatible types in equality test");
        }
    } else if (st1_type == TStackRecordType::stString) {
        if (st2_type == TStackRecordType::stString) {
            stack_.push(!st1.svalue().compare(st2.svalue()));
        } else {
            throw std::runtime_error("Incompatible types in equality test");
        }
    } else if (st1_type == TStackRecordType::stList) {
        throw std::runtime_error("Incompatible tpes in equality test");
    } else {
        throw std::runtime_error("Incompatible tpes in equality test");
    }
}

void VM::isNotEq() {
    isEq();
    auto value = stack_.top().bvalue();
    stack_.top().setValue(!value);
}

void VM::loadSymbol(int index) {
    auto symbol = symboltable().get(index);
    switch (symbol.type()) {
        case TSymbolElementType::Undefined:
            throw std::runtime_error("Undefined variable" + symbol.name());
        case TSymbolElementType::Integer:
            stack_.push(symbol.ivalue());
            break;
        case TSymbolElementType::Boolean:
            stack_.push(symbol.bvalue());
            break;
        case TSymbolElementType::Double:
            stack_.push(symbol.dvalue());
            break;
        case TSymbolElementType::String:
            stack_.push(symbol.svalue());
            break;
        case TSymbolElementType::List:
            stack_.push(symbol.lvalue());
            break;
        case TSymbolElementType::UserFunc:
            throw std::runtime_error("User function");
    }
}