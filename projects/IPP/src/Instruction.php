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
        $order = 1;

        $checker->lexicalCheck($instructionLineArray);
        $checker->syntaxCheck($instructionLineArray);
        array_shift($instructionLineArray);

        // Now we heave only arguments in $instructionLineArray
        foreach ($instructionLineArray as $argumentLine) {
            if (empty($argumentLine)) {
                continue;
            }

            $argument = new Argument();
            $argument->setOrder($order);
            $order++;
            $argument->parseArgument($argumentLine);
            array_push($this->__arguments, $argument);
        }
    }

    public function getArguments() {
        return $this->__arguments;
    }
}

?>
