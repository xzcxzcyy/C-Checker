Program -> ExtDefList

ExtDefList -> ExtDef ExtDefList
            | ExtDef

ExtDef -> ExtVarDef
        | FunDef

ExtVarDef -> VarDef

VarDef -> TypeSpec VarInitSeq
        | const TypeSpec VarInitSeq
        | TypeSpec const VarInitSeq

TypeSpec -> int
          | char
          | short
          | long
          | double
          | float

VarInitSeq -> VarInit , VarInitSeq
            | VarInit

VarInit -> identifier = ConstNumber
         | identifier

ConstNumber -> const_int
             | const_long
             | const_float
             | const_char

FunDef -> FunTypeSpec identifier(ParameterList) CompondStatements
        | FunTypeSpec identifier(ParameterTypeList) ;