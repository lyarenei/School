<?php

class Writer {
    private $__logger;
    private $__xmlWriter;

    public function __construct() {
        $this->__logger = new Logger();
        $this->__xmlWriter = new XMLWriter();
    }

    private function __writeArgument($argument) {
        $this->__xmlWriter->startElement('argx');
        $this->__xmlWriter->writeAttribute('type', $argument->getType());

        $this->__xmlWriter->startElement('value');
        $this->__xmlWriter->text($argument->getValue());
        $this->__xmlWriter->endElement(); // end value

        $this->__xmlWriter->endElement(); // end arg
    }

    private function __writeInstruction($parsedInstruction) {
        $this->__xmlWriter->startElement('instruction');
        $this->__xmlWriter->writeAttribute('order', $parsedInstruction->getOrder());
        $this->__xmlWriter->writeAttribute('opcode', $parsedInstruction->getOpcode());

        foreach ($parsedInstruction->getArguments() as $argument) {
            $this->__writeArgument($argument);
        }

        $this->__xmlWriter->endElement();
    }

    private function __writeProgram($parsedProgram) {
        $this->__xmlWriter->startElement('program');
        $this->__xmlWriter->writeAttribute('language', 'IPPcode18');

        //TODO: fix parsing language, so I can use this
        //$this->__xmlWriter->writeAttribute('language', $parsedProgram->getLanguage());
        foreach ($parsedProgram->getInstructions() as $instruction) {
            $this->__writeInstruction($instruction);
        }
        $this->__xmlWriter->endElement();
    }

    public function Write($parsedProgram, $outputFile) {
        $this->__xmlWriter->openMemory();
        $this->__xmlWriter->setIndent(4);
        $this->__xmlWriter->setIndentString('  ');
        $this->__xmlWriter->startDocument('1.0', 'UTF-8');

        $this->__writeProgram($parsedProgram);

        $this->__xmlWriter->endDocument();
        // TODO: refactor into method to write to any file
        echo $this->__xmlWriter->outputMemory();
    }
}

?>
