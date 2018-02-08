<?php

require_once 'Checker.php';

class Instruction {

    private $__order;
    private $__opcode;

    private $__arguments = array();

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

    public function setInstructionArguments($instructionLineArray) {
        $checker = new Checker();
        $checker->lexicalCheck($instructionLineArray[0]);
        $checker->syntaxCheck($instructionLineArray);
        array_shift($instructionLineArray);
        $this->__arguments = $instructionLineArray;
    }
}

?>
