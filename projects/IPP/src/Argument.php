<?php

class Argument {
    private $__order;
    private $__type;
    private $__value;

    public function setType($type) {
        $this->__type = $type;
    }

    public function getType() {
        return $this->__type;
    }

    public function setValue($value) {
        $this->__value = $value;
    }

    public function getValue() {
        return $this->__value;
    }

    public function setOrder($order) {
        $this->__order = $order;
    }

    public function getOrder() {
        return $this->__order;
    }

    public function parseArgument($argument) {
        $type = explode('@', $argument)[0];

        switch ($type) {
        case 'int':
        case 'bool':
        case 'string':
            $this->__type = $type;
            $this->__value = preg_split('/.*@/i', $argument)[1];
            break;

        case 'GF':
        case 'LF':
        case 'TF':
            $this->__type = 'var';
            $this->__value = $argument;
            break;

        default:
            $this->__type = 'label';
            $this->__value = $argument;
        }
        return $argument;
    }
}

?>
