<?php

// TODO: integrate with lexer class
class OpcodeTable {
    public function isKeyword($identifier) {
        $opcodeArray = array(
            'MOVE', 'CREATEFRAME', 'PUSHFRAME',
            'POPFRAME', 'DEFVAR', 'CALL', 'RETURN',
            'ADD', 'SUB', 'MUL', 'IDIV', 'LT', 'GT',
            'EQ', 'AND', 'OR', 'NOT', 'INT2CHAR',
            'STR2INT', 'READ', 'WRITE', 'CONCAT',
            'STRLEN', 'GETCHAR', 'SETCHAR', 'TYPE',
            'LABEL', 'JUMP', 'JUMPIFEQ', 'JUMPIFNEQ',
            'DPRINT', 'BREAK',
        );

        foreach ($opcodeArray as $opcode) {
            if ($opcode == $identifier) {
                return TRUE;
            }
        }
        return FALSE;
    }
}

?>
