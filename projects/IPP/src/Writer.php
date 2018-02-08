<?php

class Writer {
    private $__logger;
    private $__xmlWriter;

    public function __construct() {
        $this->__logger = new Logger();
        $this->__xmlWriter = new XMLWriter();
    }

    private function __WriteProgram($parsedProgram) {

    }

    public function Write($parsedProgram, $outputFile) {
        $this->__xmlWriter->openMemory();
        $this->__xmlWriter->setIndent(4);
        $this->__xmlWriter->setIndentString(' ');
        $this->__xmlWriter->startDocument('1.0', 'UTF-8');

        $this->__WriteProgram($parsedProgram);

        $this->__xmlWriter->endDocument();
        // TODO: refactor into method to write to any file
        echo $this->__xmlWriter->outputMemory();
    }
}

?>
