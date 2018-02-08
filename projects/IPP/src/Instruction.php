<?php

require_once 'Checker.php';
require_once 'Argument.php';
require_once 'Logger.php';

class Instruction {

    private $__order;
    private $__opcode;
    private $__arguments = array();

    private $__logger;

    public function __construct() {
        $this->__logger = new Logger();
    }

    private function __parseArgument($argumentLine) {
        $argument = new Argument();
        switch ($this->__opcode) {
        case 'LABEL':
        case 'JUMP':
            $argument->setType('label');
            $argument->setValue($argumentLine);
            break;

        default:
            $argument->setType('unknown');
            $argument->setValue($argumentLine);
        }

        return $argument;
    }

    public function setOrder($order) {
        $this->__order = $order;
    }

    public function getOrder() {
        return $this->__order;
    }

    public function setOpcode($opcode) {
        $this->__opcode = $opcode;
    }

    public function getOpcode() {
        return $this->__opcode;
    }

    public function setArguments($instructionLineArray) {
        $checker = new Checker();
        $checker->lexicalCheck($instructionLineArray[0]);
        $checker->syntaxCheck($instructionLineArray);
        array_shift($instructionLineArray);

        // Now we heave only arguments in $instructionLineArray
        foreach ($instructionLineArray as $argumentLine) {
            $argument = $this->__parseArgument($argumentLine);
            array_push($this->__arguments, $argument);
        }
    }

    public function getArguments() {
        return $this->__arguments;
    }
}

?>
