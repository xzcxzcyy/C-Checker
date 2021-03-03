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

// ParameterList and ParameterTypeList are optional
FunDef -> FunTypeSpec identifier ( ParameterList ) CompoundStatements
        | FunTypeSpec identifier ( ParameterTypeList ) ;

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

CompoundStatements -> { Statements }
                    | { }

Statements -> Statement Statements
            | Statement

Statement -> Expression ;
           | CompoundStatements
           | IfStatement
           | WhileStatement
           | ForStatement
           | ReturnStatement
           | BreakStatement
           | ContinueStatement
           | LocalVarDef
           | ;

LocalVarDef -> VarDef

Expression -> 

WrappedExpression -> ( Expression )

IfStatement -> if ( Expression ) Statement else Statement
             | if ( Expression ) Statement

WhileStatement -> while ( Expression ) Statement

ForStatement -> for ( LocalVarDef Expression ; Expression ) Statement
              | for ( Expression ; Expression ; Expression ) Statement

ReturnStatement -> return Expression ;

BreakStatement -> break ;

ContinueStatement -> continue ;

FunctionCall -> identifier ( )
              | identifier ( ArgumentList )

ArgumentList -> Expression , ArgumentList
              | Expression
