<?php
// TODO: refactor and split

require_once 'OpcodeTable.php';
require_once 'Logger.php';

define("LEXICAL_ERROR_CODE", 21);
define("SYNTAX_ERROR_CODE", 21);

class Checker {

    private $__opcodeTable;
    private $__logger;

    private function __matchOpcode($opcode) {
        if ($this->__opcodeTable->IsKeyword($opcode) == false) {
            $this->__logger->PrintError('Lexical analysis: "' . $opcode . '" is not valid instruction.', LEXICAL_ERROR_CODE);
        }
    }

    private function __checkArgumentCount($instructionArray) {
        $argumentCount = count($instructionArray) - 1; // first is opcode
        if ($argumentCount > 3) {
            $this->__logger->PrintError('Syntax analysis: "' . $instructionArray[0] . '" has too many arguments.', SYNTAX_ERROR_CODE);
        }
        switch ($instructionArray[0]) {
        // 0 args
        case 'CREATEFRAME':
        case 'PUSHFRAME':
        case 'POPFRAME':
        case 'BREAK':
        case 'RETURN':
        case 'BREAK':
            if ($argumentCount != 0) {
                $this->__logger->PrintError('Syntax analysis: "' . $instructionArray[0] . '" has too few / many arguments.', SYNTAX_ERROR_CODE);
            }
            break;

        // 1 arg
        case 'DEFVAR':
        case 'CALL':
        case 'PUSHS':
        case 'POPS':
        case 'WRITE':
        case 'LABEL':
        case 'JUMP':
        case 'DPRINT':
            if ($argumentCount != 1) {
                $this->__logger->PrintError('Syntax analysis: "' . $instructionArray[0] . '" has too few / many arguments.', SYNTAX_ERROR_CODE);
            }
            break;

        // 2 args
        case 'MOVE':
        case 'INT2CHAR':
        case 'READ':
        case 'STRLEN':
        case 'TYPE':
            if ($argumentCount != 2) {
                $this->__logger->PrintError('Syntax analysis: "' . $instructionArray[0] . '" has too few / many arguments.', SYNTAX_ERROR_CODE);
            }
            break;

        // 3 args
        case 'ADD':
        case 'SUB':
        case 'MUL':
        case 'IDIV':
        case 'LT':
        case 'GT':
        case 'EQ':
        case 'AND':
        case 'OR':
        case 'NOT':
        case 'STR2INT':
        case 'CONCAT':
        case 'GETCHAR':
        case 'SETCHAR':
        case 'JUMPIFEQ':
        case 'JUMPIFNEQ':
            if ($argumentCount != 3) {
                $this->__logger->PrintError('Syntax analysis: "' . $instructionArray[0] . '" has too few / many arguments.', SYNTAX_ERROR_CODE);
            }
            break;
        }
    }

    public function __construct() {
        $this->__opcodeTable = new OpcodeTable();
        $this->__logger = new Logger();
    }

    public function LexicalCheck($instructionArray) {
        $this->__matchOpcode($instructionArray[0]);
    }

    public function SyntaxCheck($instructionArray) {
        $this->__checkArgumentCount($instructionArray);
    }
}

?>
