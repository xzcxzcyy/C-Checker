Program -> ExtDefList

ExtDefList -> ExtDef ExtDefList
            | ExtDef

ExtDef -> ExtVarDef
        | FunDef

ExtVarDef -> VarDef

VarDef -> TypeSpec VarInitSeq ;
        | const TypeSpec VarInitSeq ;
        | TypeSpec const VarInitSeq ;

TypeSpec -> int
          | char
          | short
          | long
          | double
          | float

VarInitSeq -> VarInit , VarInitSeq
            | VarInit

VarInit -> identifier = Expression
         | identifier [ const_int ]
         | identifier

ConstNumber -> const_int
             | const_long
             | const_float
             | const_char

FunDef -> FunTypeSpec identifier(ParameterList) CompondStatements
        | FunTypeSpec identifier(ParameterTypeList) ;

FunTypeSpec -> void
             | int
             | char
             | short
             | long
             | double
             | float

ParameterList -> Parameter , ParameterList
               | Parameter

Parameter -> TypeSpec identifier

ParameterTypeList -> ParameterType , ParameterTypeList
                   | ParameterType

ParameterType -> TypeSpec identifier
               | TypeSpec

CompondStatements -> { Statements }
                   | { }

Statements -> Statement Statements
            | Statement

Statement -> Expression ;
           | CompondStatements
           | IfStatement
           | WhileStatement
           | ForStatement
           | return Expression ;
           | break ;
           | continue ;
           | LocalVarDef
           | ;

LocalVarDef -> VarDef

Expression -> 

IfStatement -> if ( Expression ) Statement else Statement
             | if ( Expression ) Statement

WhileStatement -> while ( Expression ) Statement

ForStatement -> for ( LocalVarDef Expression ; Expression ) Statement
              | for ( Expression ; Expression ; Expression ) Statement