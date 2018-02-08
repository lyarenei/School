<?php

class Argument {
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
}

?>
