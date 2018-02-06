-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2017 Brno University of Technology,
--                    Faculty of Information Technology
-- Author(s): Dominik Krivohlavek <xkrivo02>
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
    port (
        CLK   : in std_logic;  -- hodinovy signal
        RESET : in std_logic;  -- asynchronni reset procesoru
        EN    : in std_logic;  -- povoleni cinnosti procesoru

        -- synchronni pamet ROM
        CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
        CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
        CODE_EN   : out std_logic;                     -- povoleni cinnosti

        -- synchronni pamet RAM
        DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
        DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
        DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
        DATA_RDWR  : out std_logic;                    -- cteni z pameti (DATA_RDWR='0') / zapis do pameti (DATA_RDWR='1')
        DATA_EN    : out std_logic;                    -- povoleni cinnosti

        -- vstupni port
        IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA obsahuje stisknuty znak klavesnice pokud IN_VLD='1' a IN_REQ='1'
        IN_VLD    : in std_logic;                      -- data platna pokud IN_VLD='1'
        IN_REQ    : out std_logic;                     -- pozadavek na vstup dat z klavesnice

        -- vystupni port
        OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
        OUT_BUSY : in std_logic;                       -- pokud OUT_BUSY='1', LCD je zaneprazdnen, nelze zapisovat,  OUT_WE musi byt '0'
        OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
    );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

    -- Signal for PC operation
    -- 00 = do nothing
    -- 01 = increment
    -- 10 = decrement
    -- 11 = do nothing
    signal PC_op : std_logic_vector(1 downto 0) := "00";

    -- Signal for PTR operation
    -- 00 = do nothing
    -- 01 = increment
    -- 10 = decrement
    -- 11 = do nothing
    signal PTR_op : std_logic_vector(1 downto 0) := "00";

    -- Signal for data multiplexor switching
    -- 00 = Zero output
    -- 01 = increment input
    -- 10 = decrement input
    -- 11 = IN_DATA passthrough
    signal MUX_op : std_logic_vector(1 downto 0) := "11";

    -- Controller states for instructions
    type instruction_type is (
        ptr_inc, ptr_dec,                   -- Memory pointer increment '>' / decrement '<'
        val_inc, val_inc_op,                -- Value increment, increment operation '+',
        val_dec, val_dec_op,                -- Value decrement, decrement operation '-'
        loop_start, loop_inc_PC,            -- Loop start, loop body '['
        loop_end, loop_dec_PC,              -- Loop end, loop body ']'
        print_out, read_in,                 -- Print out memory value '.' / Read user input ','
        loop_break, loop_break_inc_PC,      -- Break loop, loop body '~'
        halt, noop,                         -- Stop execution / No operation
        get_instr, read_instr,              -- Move to next instruction / get instruction
        unknown_instr                       -- Unknown instruction
    );

    -- Signals for controller states
    signal present_state, next_state, instr_type, instr_tmp : instruction_type := unknown_instr;

begin

-- ----------------------------------------------------------------------------
--                             CPU registers
-- ----------------------------------------------------------------------------

    -- CPU Program Counter register
    program_counter : process (RESET, CLK, PC_op)
    variable PC : std_logic_vector(11 downto 0) := "000000000000";
    begin
        if (RESET = '1') then
            PC := "000000000000";
        elsif (CLK = '1' and CLK'event and EN = '1') then
            if (PC_op = "01") then
                PC := PC + '1';
            elsif (PC_op = "10") then
                PC := PC - '1';
            end if;
        end if;
        CODE_ADDR <= PC;
    end process program_counter;

    -- CPU Memory pointer register
    memory_pointer : process (RESET, CLK, PTR_op)
    variable PTR : std_logic_vector(9 downto 0) := "0000000000";
    begin
        if (RESET = '1') then
            PTR := "0000000000";
        elsif (CLK = '1' and CLK'event and EN = '1') then
            if (PTR_op = "01") then
                PTR := PTR + '1';
            elsif (PTR_op = "10") then
                PTR := PTR - '1';
            end if;
        end if;
        DATA_ADDR <= PTR;
    end process memory_pointer;

-- ----------------------------------------------------------------------------
--                          Other components
-- ----------------------------------------------------------------------------

    -- Multiplexor for data modification
    data_mux : process (MUX_op, DATA_RDATA, IN_DATA) begin
        case MUX_op is
            when "00" => DATA_WDATA <= "00000000";
            when "01" => DATA_WDATA <= DATA_RDATA + '1';
            when "10" => DATA_WDATA <= DATA_RDATA - '1';
            when "11" => DATA_WDATA <= IN_DATA;
            when others => null;
        end case;
    end process data_mux;

-- ----------------------------------------------------------------------------
--                          CPU Controller
-- ----------------------------------------------------------------------------

    -- State switcher for controller
    controller_switch : process (CLK, RESET) begin
        if RESET = '1' then
            present_state <= noop;
        elsif (CLK'event and CLK='1' and EN = '1') then
            present_state <= next_state;
        end if;
    end process controller_switch;

    -- Instruction decoder
    instruction_decoder : process (CODE_DATA) begin
        case CODE_DATA is
            when "00111110" => instr_type <= ptr_inc;           -- >
            when "00111100" => instr_type <= ptr_dec;           -- <
            when "00101011" => instr_type <= val_inc;           -- +
            when "00101101" => instr_type <= val_dec;           -- -
            when "01011011" => instr_type <= loop_start;        -- [
            when "01011101" => instr_type <= loop_end;          -- ]
            when "00101110" => instr_type <= print_out;         -- .
            when "00101100" => instr_type <= read_in;           -- ,
            when "01111110" => instr_type <= loop_break;        -- ~
            when "00000000" => instr_type <= halt;              -- null
            when others => instr_type <= unknown_instr;
        end case;
    end process instruction_decoder;

    -- Instruction functionality and state switching
    instruction_func : process (present_state, CLK, CODE_DATA, DATA_RDATA, OUT_BUSY, IN_VLD, instr_type) begin
        -- Define everything, so every state will only need to modify what it needs
        CODE_EN <= '1';
        PC_op <= "00";                                                  -- do not increment PC
        PTR_op <= "00";                                                 -- do not modify PTR
        MUX_op <= "00";                                                 -- "switch off" multiplexor for RAM DATA
        DATA_EN <= '0';                                                 -- disable RAM operation
        DATA_RDWR <= '0';                                               -- Set RAM operation to read
        IN_REQ <= '0';                                                  -- no input request
        OUT_WE <= '0';                                                  -- no output writing
        OUT_DATA <= "00000000";
        next_state <= noop;

        case present_state is
            when get_instr =>
                PC_op <= "01";
                next_state <= read_instr;

            when read_instr =>
                case instr_type is
                    when ptr_inc => next_state <= ptr_inc;              -- >
                    when ptr_dec => next_state <= ptr_dec;              -- <
                    when val_inc => next_state <= val_inc;              -- +
                    when val_dec => next_state <= val_dec;              -- -
                    when loop_start => next_state <= loop_start;        -- [
                    when loop_end => next_state <= loop_end;            -- ]
                    when print_out => next_state <= print_out;          -- .
                    when read_in => next_state <= read_in;              -- ,
                    when loop_break => next_state <= loop_break;        -- ~
                    when halt => next_state <= halt;                    -- null
                    when others => next_state <= unknown_instr;
                end case;

            -- Instruction functionalities
            when ptr_inc =>
                PTR_op <= "01";
                next_state <= get_instr;

            when ptr_dec =>
                PTR_op <= "10";
                next_state <= get_instr;

            when val_inc =>
                DATA_EN <= '1';                                         -- read from memory to operate with right value
                next_state <= val_inc_op;                               -- write chenged value

            when val_inc_op =>
                DATA_EN <= '1';
                MUX_op <= "01";                                         -- Increment input value
                DATA_RDWR <= '1';
                next_state <= get_instr;

            when val_dec =>
                DATA_EN <= '1';                                         -- read from memory to operate with right value
                next_state <= val_dec_op;                               -- write changed value

            when val_dec_op =>
                DATA_EN <= '1';
                MUX_op <= "10";                                         -- Decrement input value
                DATA_RDWR <= '1';
                next_state <= get_instr;

            when print_out =>
                if OUT_BUSY = '0' then
                    DATA_RDWR <= '0';
                    DATA_EN <= '1';
                    OUT_DATA <= DATA_RDATA;
                    OUT_WE <= '1';
                    next_state <= get_instr;
                else
                    next_state <= print_out;
                end if;

            when read_in =>
                IN_REQ <= '1';
                if IN_VLD = '1' then
                    DATA_EN <= '1';
                    MUX_op <= "11";                                     -- Passthrough IN_DATA
                    DATA_RDWR <= '1';
                    next_state <= get_instr;
                else
                    next_state <= read_in;
                end if;

            when loop_start =>
                DATA_EN <= '1';
                if (DATA_RDATA = "00000000") then
                    next_state <= loop_inc_PC;
                else
                    next_state <= get_instr;
                end if;

            when loop_inc_PC =>
                PC_op <= "01";
                if (instr_type = loop_end) then
                    next_state <= get_instr;
                else
                    next_state <= loop_inc_PC;
                end if;

            when loop_end =>
                DATA_EN <= '1';
                if (DATA_RDATA /= "00000000") then
                    next_state <= loop_dec_PC;
                else
                    next_state <= get_instr;
                end if;

            when loop_dec_PC =>
                PC_op <= "10";
                if (instr_type = loop_start) then
                    PC_op <= "01";
                    next_state <= get_instr;
                else
                    next_state <= loop_dec_PC;
                end if;

            when loop_break =>
                next_state <= loop_break_inc_PC;

            when loop_break_inc_PC =>
                PC_op <= "00";
                if (instr_type = loop_end) then
                    next_state <= get_instr;
                else
                    PC_op <= "01";
                    next_state <= loop_break_inc_PC;
                end if;

            when halt => next_state <= halt;

            when others => next_state <= get_instr;

        end case;
    end process instruction_func;

end behavioral;
