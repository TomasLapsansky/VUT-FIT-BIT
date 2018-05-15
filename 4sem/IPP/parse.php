<?php

# Tomas Lapsansky (xlapsa00)
# VUT FIT 2017/2018

class Token
{
    public $keyword;
    public $str;

    function __Construct($keyword, $str) {
        $this->keyword = $keyword;
        $this->str = $str;
    }
}

class _Global
{
    public static $stdin;
    public static $xml;
    private static $xml_program;
    public static $instruction_counter = 0;
    public static $stats_comments = 0;

    /**
     * Exiting program with specific return code
     * @param string $message Message to be printed on STDERR
     * @param integer $exit_code Exit code of program
     */
    public static function forceQuit($message, $exit_code) {

        fclose(_Global::$stdin);

        $message .= "\n";
        fwrite(STDERR, $message);

        gc_collect_cycles();

        exit($exit_code);
    }

    /**
     * Generating help for users
     */
    public static function printHelp() {

        echo("Parse.php is parsing input code (IPPcode18) to the output xml format which can be used with interpret.py\n\n");
        echo("Parameters:\n");
        echo("--help\t\tprinting help\n");
        echo("--stats=\"file\"\tprinting statistics to the FILE\n");
        echo("--loc\t\thas to be used with --stats, printing lines of code to the FILE\n");
        echo("--comments\thas to be used with --stats, printing number of comments in input file\n");
        exit(0);
    }

    /**
     * Initialization of output XML file
     */
    public static function initXML() {
        _Global::$xml = new DomDocument("1.0", "UTF-8");
        _Global::$xml_program = _Global::$xml->createElement('program');
        _Global::$xml_program->setAttribute('language', 'IPPcode18');
        _Global::$xml->appendChild(_Global::$xml_program);

    }

    /**
     * Adding to output XML file
     */
    public static function addToXML()
    {
        if(is_object(_Global::$xml)) {

            $name_token = func_get_arg(0);

            $instruction = _Global::$xml->createElement('instruction');
            $instruction->setAttribute('order', _Global::$instruction_counter);
            $instruction->setAttribute('opcode', $name_token);

            for($i = 1; $i < func_num_args(); $i++) {

                $arg_token = func_get_arg($i);

                #switch for attributes
                switch ($arg_token->keyword) {

                    case "var":
                        $arg = _Global::$xml->createElement("arg$i", $arg_token->str);
                        $arg->setAttribute('type', 'var');
                        break;
                    case "label":
                        $arg = _Global::$xml->createElement("arg$i", $arg_token->str);
                        $arg->setAttribute('type', 'label');
                        break;
                    case "type":
                        $arg = _Global::$xml->createElement("arg$i", $arg_token->str);
                        $arg->setAttribute('type', 'type');
                        break;
                    case "symb_int":

                        $edited_token_str = substr($arg_token->str, 4);

                        $arg = _Global::$xml->createElement("arg$i", $edited_token_str);
                        $arg->setAttribute('type', 'int');

                        break;

                    case "symb_bool":

                        $edited_token_str = substr($arg_token->str, 5);

                        $arg = _Global::$xml->createElement("arg$i", $edited_token_str);
                        $arg->setAttribute('type', 'bool');

                        break;

                    case "symb_string":

                        $edited_token_str = substr($arg_token->str, 7);

                        $arg = _Global::$xml->createElement("arg$i", $edited_token_str);
                        $arg->setAttribute('type', 'string');

                        break;

                    default:

                        _Global::forceQuit("Internal addToXML error", 99);

                }

                $instruction->appendChild($arg);
            }

            _Global::$xml_program->appendChild($instruction);
        }
    }
}

class Lexical
{
    private static $temp_lex;
    private static $temp_char;
    public static $keywords = array("MOVE", "CREATEFRAME", "PUSHFRAME", "POPFRAME", "DEFVAR", "CALL", "RETURN", "PUSHS", "POPS", "ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "NOT", "INT2CHAR", "STRI2INT", "READ", "WRITE", "CONCAT", "STRLEN", "GETCHAR", "SETCHAR", "TYPE", "LABEL", "JUMP", "JUMPIFEQ", "JUMPIFNEQ", "DPRINT", "BREAK");

    /**
     * Determining if lexem is keyword or not
     * @return bool|Token
     */
    private static function isKeyword() {

        foreach (Lexical::$keywords as $keyword) {
            if(strcasecmp(Lexical::$temp_lex, $keyword) == 0) {    #case insensitive comp

                $token = new Token($keyword, $keyword);
                _Global::$instruction_counter++;
                return $token;
            }
        }

        return false;
    }

    /**
     * Loads one character from stdin
     */
    private static function loadChar() {   # BEAST for comments
        Lexical::$temp_char = fgetc(_Global::$stdin);

        # comments removing
        # getting Char as long as it gets EOL :-)
        if(Lexical::$temp_char === '#') {
            _Global::$stats_comments++;
            while(ord(Lexical::$temp_char) != 10) {
                Lexical::$temp_char = fgetc(_Global::$stdin);
            }
        }
    }

    /**
     * Getting token from stdin
     * @return bool|Token
     */
    public static function getToken()
    {
        #temporary
        Lexical::$temp_lex = '';

        $token = false;

        ## Working with whitespaces
        do {
            $test_again = false;
            if(ord(Lexical::$temp_char) == 10) {               #EOL is loaded

                Lexical::loadChar();

                $token = new Token("EOL", "EOL");
                $test_again = false;

            } elseif(feof(_Global::$stdin)) {                  #EOF test

                $token = new Token("EOF", "EOF");

            } elseif(ctype_space(Lexical::$temp_char)) {       #whitespace remover

                Lexical::loadChar();
                $test_again = true;

            }
        } while($test_again);

        while($token == false) {

            if(ctype_space(Lexical::$temp_char)) {
                if (Lexical::$temp_lex === ".IPPcode18") {
                    $token = new Token(".IPPcode18", ".IPPcode18");
                } else if(($token = Lexical::isKeyword()) != false) {
                    ;   #empty, isKeyword is doing everything :-)
                } elseif(preg_match('/^(GF|LF|TF)@([a-z]|[A-Z]|[\_\-\$\&\%\*])([a-z]|[A-Z]|[0-9]|[\_\-\$\&\%\*])*$/', Lexical::$temp_lex) == 1) {
                    $token = new Token("var", Lexical::$temp_lex);          #$var as GF/LF/TF@name_of_var - can't start with number

                } elseif(preg_match('/^int@[+-]?[0-9]+$/', Lexical::$temp_lex) == 1) {
                    $token = new Token("symb_int", Lexical::$temp_lex);     #const without $var - this could contain $var

                } elseif(preg_match('/^bool@(true|false)$/', Lexical::$temp_lex) == 1) {
                    $token = new Token("symb_bool", Lexical::$temp_lex);    #const without $var - this could contain $var

                } elseif(preg_match('/^string@([a-z]|[A-Z]|[0-9]|\\\\[0-9]{3}|[\_\-\$\&\%\*])*$/', Lexical::$temp_lex) == 1) {
                    $token = new Token("symb_string", Lexical::$temp_lex);  #const without $var - this could contain $var

                } elseif(preg_match('/^(int|string|bool)$/', Lexical::$temp_lex) == 1) {
                    $token = new Token( "type", Lexical::$temp_lex);        #type

                } elseif(preg_match('/^([a-z]|[A-Z]|[\_\-\$\&\%\*])([a-z]|[A-Z]|[0-9]|[\_\-\$\&\%\*])*$/', Lexical::$temp_lex) == 1) {
                    $token = new Token( "label", Lexical::$temp_lex);       #same as $var, without FRAME@

                } else {
                    _Global::forceQuit("Lexical error", 21);
                }

            } else {
                Lexical::$temp_lex .= Lexical::$temp_char;

                Lexical::loadChar();

            }
        }

        return $token;
    }
}

class Syntax
{
    /*
    # Grammar
    #
    # 01    .IPPcode18 EOL <instruction> EOF
    #   <instruction>
    # 02    MOVE <var> <symb> EOL <instruction>
    # 03    CREATEFRAME EOL <instruction>
    # 04    PUSHFRAME EOL <instruction>
    # 05    POPFRAME EOL <instruction>
    # 06    DEFVAR <var> EOL <instruction>
    # 07    CALL <label> EOL <instruction>
    # 08    RETURN EOL <instruction>
    # 09    PUSHS <symb> EOL <instruction>
    # 10    POPS <var> EOL <instruction>
    # 11    ADD <var> <symb1> <symb2> EOL <instruction>
    # 12    SUB <var> <symb1> <symb2> EOL <instruction>
    # 13    MUL <var> <symb1> <symb2> EOL <instruction>
    # 14    IDIV <var> <symb1> <symb2> EOL <instruction>
    # 15    LT <var> <symb1> <symb2> EOL <instruction>
    # 16    GT <var> <symb1> <symb2> EOL <instruction>
    # 17    EQ <var> <symb1> <symb2> EOL <instruction>
    # 18    AND <var> <symb1> <symb2> EOL <instruction>
    # 19    OR <var> <symb1> <symb2> EOL <instruction>
    # 20    NOT <var> <symb1> <symb2> EOL <instruction>
    # 21    INT2CHAR <var> <symb> EOL <instruction>
    # 22    STRI2INT <var> <symb1> <symb2> EOL <instruction>
    # 23    READ <var> <type> EOL <instruction>
    # 24    WRITE <symb> EOL <instruction>
    # 25    CONCAT <var> <symb1> <symb2> EOL <instruction>
    # 26    STRLEN <var> <symb> EOL <instruction>
    # 27    GETCHAR <var> <symb1> <symb2> EOL <instruction>
    # 28    SETCHAR <var> <symb1> <symb2> EOL <instruction>
    # 29    TYPE <var> <symb> EOL <instruction>
    # 30    LABEL <label> EOL <instruction>
    # 31    JUMP <label> EOL <instruction>
    # 32    JUMPIFEQ <label> <symb1> <symb2> EOL <instruction>
    # 33    JUMPIFNEQ <label> <symb1> <symb2> EOL <instruction>
    # 34    DPRINT <symb> EOL <instruction>
    # 35    BREAK EOL <instruction>
    */

    /**
     * Start of syntax analyzator
     */
    public static function syntax_analysis() {

        Syntax::p_start();
    }

    /**
     * Checking the type of token with $keyword and $str
     * @param $keyword
     * @param $str
     * @param bool $exit determining if program will be exited, if there is an error
     * @return bool|Token
     */
    private static function checkToken($keyword, $str, $exit)
    {

        $token = Lexical::getToken();

        #converting $token->keyword to general $keyword
        if($token->keyword === 'symb_int' || $token->keyword === 'symb_bool' || $token->keyword === 'symb_string') {

            $general_keyword = 'symb';

        } else {
            $general_keyword = $token->keyword;
        }

        if($keyword === '' && $str === '') {

            _Global::forceQuit("Internal checkToken error", 99);

        } elseif($keyword != '') {

            if(strcmp($keyword, $general_keyword) != 0) {
                if ($keyword === 'symb' && $general_keyword === 'var') {        #correct functioning symb can be var
                    ;
                } elseif($exit) {
                    _Global::forceQuit("Syntax error", 21);
                } else {
                    return false;
                }
            }

        } elseif($str != '')  {

            if(strcmp($str, $token->str) != 0) {
                if($exit) {
                    _Global::forceQuit("Syntax error", 21);
                } else {
                    return false;
                }
            }
        }

        return $token;
    }

    /**
     * Instruction package
     */
    private static function instruction() {

        $token = Lexical::getToken();

        switch (strtoupper($token->keyword)) {      #case insensitive compare

            case "MOVE":
                Syntax::p_MOVE();
                break;
            case "CREATEFRAME":
                Syntax::p_CREATEFRAME();
                break;
            case "PUSHFRAME":
                Syntax::p_PUSHFRAME();
                break;
            case "POPFRAME":
                Syntax::p_POPFRAME();
                break;
            case "DEFVAR":
                Syntax::p_DEFVAR();
                break;
            case "CALL":
                Syntax::p_CALL();
                break;
            case "RETURN":
                Syntax::p_RETURN();
                break;
            case "PUSHS":
                Syntax::p_PUSHS();
                break;
            case "POPS":
                Syntax::p_POPS();
                break;
            case "ADD":
                Syntax::p_ADD();
                break;
            case "SUB":
                Syntax::p_SUB();
                break;
            case "MUL":
                Syntax::p_MUL();
                break;
            case "IDIV":
                Syntax::p_IDIV();
                break;
            case "LT":
                Syntax::p_LT();
                break;
            case "GT":
                Syntax::p_GT();
                break;
            case "EQ":
                Syntax::p_EQ();
                break;
            case "AND":
                Syntax::p_AND();
                break;
            case "OR":
                Syntax::p_OR();
                break;
            case "NOT":
                Syntax::p_NOT();
                break;
            case "INT2CHAR":
                Syntax::p_INT2CHAR();
                break;
            case "STRI2INT":
                Syntax::p_STRI2INT();
                break;
            case "READ":
                Syntax::p_READ();
                break;
            case "WRITE":
                Syntax::p_WRITE();
                break;
            case "CONCAT":
                Syntax::p_CONCAT();
                break;
            case "STRLEN":
                Syntax::p_STRLEN();
                break;
            case "GETCHAR":
                Syntax::p_GETCHAR();
                break;
            case "SETCHAR":
                Syntax::p_SETCHAR();
                break;
            case "TYPE":
                Syntax::p_TYPE();
                break;
            case "LABEL":
                Syntax::p_LABEL();
                break;
            case "JUMP":
                Syntax::p_JUMP();
                break;
            case "JUMPIFEQ":
                Syntax::p_JUMPIFEQ();
                break;
            case "JUMPIFNEQ":
                Syntax::p_JUMPIFNEQ();
                break;
            case "DPRINT":
                Syntax::p_DPRINT();
                break;
            case "BREAK":
                Syntax::p_BREAK();
                break;
            case "EOL":     #for empty lines
                Syntax::instruction();
                break;
            default:
                ; #end instruction state
        }

    }

    # 01    .IPPcode18 EOL <instruction> EOF
    private static function p_start() {

        Syntax::checkToken('', '.IPPcode18', true);
        Syntax::checkToken('EOL', 'EOL', true);
        Syntax::instruction();
        Syntax::checkToken('EOF', 'EOF', true);

    }

    # 02    MOVE <var> <symb> EOL <instruction>
    private static function p_MOVE() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("MOVE", $arg1, $arg2);

        Syntax::instruction();

    }

    # 03    CREATEFRAME EOL <instruction>
    private static function p_CREATEFRAME() {

        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("CREATEFRAME");

        Syntax::instruction();

    }

    # 04    PUSHFRAME EOL <instruction>
    private static function p_PUSHFRAME() {

        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("PUSHFRAME");

        Syntax::instruction();

    }

    # 05    POPFRAME EOL <instruction>
    private static function p_POPFRAME() {

        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("POPFRAME");

        Syntax::instruction();

    }

    # 06    DEFVAR <var> EOL <instruction>
    private static function p_DEFVAR() {

        $arg1 = Syntax::checkToken('var', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("DEFVAR", $arg1);

        Syntax::instruction();

    }

    # 07    CALL <label> EOL <instruction>
    private static function p_CALL() {

        $arg1 = Syntax::checkToken('label', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("CALL", $arg1);

        Syntax::instruction();

    }

    # 08    RETURN EOL <instruction>
    private static function p_RETURN() {

        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("RETURN");

        Syntax::instruction();

    }

    # 09    PUSHS <symb> EOL <instruction>
    private static function p_PUSHS() {

        $arg1 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("PUSHS", $arg1);

        Syntax::instruction();

    }

    # 10    POPS <var> EOL <instruction>
    private static function p_POPS() {

        $arg1 = Syntax::checkToken('var', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("POPS", $arg1);

        Syntax::instruction();

    }

    # 11    ADD <var> <symb1> <symb2> EOL <instruction>
    private static function p_ADD() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("ADD", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 12    SUB <var> <symb1> <symb2> EOL <instruction>
    private static function p_SUB() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("SUB", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 13    MUL <var> <symb1> <symb2> EOL <instruction>
    private static function p_MUL() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("MUL", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 14    IDIV <var> <symb1> <symb2> EOL <instruction>
    private static function p_IDIV() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("IDIV", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 15    LT <var> <symb1> <symb2> EOL <instruction>
    private static function p_LT() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("LT", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 16    GT <var> <symb1> <symb2> EOL <instruction>
    private static function p_GT() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("GT", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 17    EQ <var> <symb1> <symb2> EOL <instruction>
    private static function p_EQ() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("EQ", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 18    AND <var> <symb1> <symb2> EOL <instruction>
    private static function p_AND() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("AND", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 19    OR <var> <symb1> <symb2> EOL <instruction>
    private static function p_OR() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("OR", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 20    NOT <var> <symb1> <symb2> EOL <instruction>
    private static function p_NOT() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("NOT", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 21    INT2CHAR <var> <symb> EOL <instruction>
    private static function p_INT2CHAR() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("INT2CHAR", $arg1, $arg2);

        Syntax::instruction();

    }

    # 22    STRI2INT <var> <symb1> <symb2> EOL <instruction>
    private static function p_STRI2INT() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("STRI2INT", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 23    READ <var> <type> EOL <instruction>
    private static function p_READ() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('type', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("READ", $arg1, $arg2);

        Syntax::instruction();

    }

    # 24    WRITE <symb> EOL <instruction>
    private static function p_WRITE() {

        $arg1 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("WRITE", $arg1);

        Syntax::instruction();

    }

    # 25    CONCAT <var> <symb1> <symb2> EOL <instruction>
    private static function p_CONCAT() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("CONCAT", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 26    STRLEN <var> <symb> EOL <instruction>
    private static function p_STRLEN() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("STRLEN", $arg1, $arg2);

        Syntax::instruction();

    }

    # 27    GETCHAR <var> <symb1> <symb2> EOL <instruction>
    private static function p_GETCHAR() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("GETCHAR", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 28    SETCHAR <var> <symb1> <symb2> EOL <instruction>
    private static function p_SETCHAR() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("SETCHAR", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 29    TYPE <var> <symb> EOL <instruction>
    private static function p_TYPE() {

        $arg1 = Syntax::checkToken('var', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("TYPE", $arg1, $arg2);

        Syntax::instruction();

    }

    # 30    LABEL <label> EOL <instruction>
    private static function p_LABEL() {

        $arg1 = Syntax::checkToken('label', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("LABEL", $arg1);

        Syntax::instruction();

    }

    # 31    JUMP <label> EOL <instruction>
    private static function p_JUMP() {

        $arg1 = Syntax::checkToken('label', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("JUMP", $arg1);

        Syntax::instruction();

    }

    # 32    JUMPIFEQ <label> <symb1> <symb2> EOL <instruction>
    private static function p_JUMPIFEQ() {

        $arg1 = Syntax::checkToken('label', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("JUMPIFEQ", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 33    JUMPIFNEQ <label> <symb1> <symb2> EOL <instruction>
    private static function p_JUMPIFNEQ() {

        $arg1 = Syntax::checkToken('label', '', true);
        $arg2 = Syntax::checkToken('symb', '', true);
        $arg3 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("JUMPIFNEQ", $arg1, $arg2, $arg3);

        Syntax::instruction();

    }

    # 34    DPRINT <symb> EOL <instruction>
    private static function p_DPRINT() {

        $arg1 = Syntax::checkToken('symb', '', true);
        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("DPRINT", $arg1);

        Syntax::instruction();

    }

    # 35    BREAK EOL <instruction>
    private static function p_BREAK() {

        Syntax::checkToken('EOL', 'EOL', true);

        _Global::addToXML("BREAK");

        Syntax::instruction();

    }


}

#main

_Global::$stdin = fopen('php://stdin', 'r');
_Global::initXML();

#arguments test
if($argc != 1) {

    $short_opts = "";       #no short opts
    $long_opts = array(
        "help",
        "stats::",
        "loc",
        "comments",
    );
    $options = getopt($short_opts, $long_opts);

    #help
    if(array_key_exists("help", $options)) {

        if($argc == 2) {
            _Global::printHelp();
            exit(0);
        } else {
            _Global::forceQuit("Wrong parameters, use --help", 10);
        }

        #stats
    } else if(array_key_exists("stats", $options)) {

        $bool_loc = false;
        $bool_comments = false;

        $stat_file = fopen($options["stats"], "w");
        if($stat_file == false) {
            _Global::forceQuit("Cannot open stats file", 12);
        }

        if(array_key_exists("loc", $options)) {
            $bool_loc = true;
        }
        if(array_key_exists("comments", $options)) {
            $bool_comments = true;
        }

        Syntax::syntax_analysis();

        fwrite($stat_file, "Statistics file\n");

        if($bool_loc) {
            fwrite($stat_file, "LOC: ");
            fwrite($stat_file, _Global::$instruction_counter);
            fwrite($stat_file, "\n");
        }
        if($bool_comments) {
            fwrite($stat_file, "Number of comments: ");
            fwrite($stat_file, _Global::$stats_comments);
            fwrite($stat_file, "\n");
        }

        fclose($stat_file);

    } else {

        _Global::forceQuit("Wrong parameters, use --help", 10);

    }

} else {

    Syntax::syntax_analysis();
}

fclose(_Global::$stdin);

_Global::$xml->formatOutput = true;
echo _Global::$xml->saveXML();

return 0;
