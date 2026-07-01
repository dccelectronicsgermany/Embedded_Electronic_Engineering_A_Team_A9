library ieee;
use ieee.std_logic_1164.all;

entity tb_reservation_check_top is
end tb_reservation_check_top;

architecture sim of tb_reservation_check_top is

    signal origin          : std_logic_vector(1 downto 0);
    signal intent          : std_logic_vector(1 downto 0);
    signal occupied_zones  : std_logic_vector(3 downto 0);

    signal requested_zones : std_logic_vector(3 downto 0);
    signal valid_route     : std_logic;
    signal conflict        : std_logic;
    signal free_slot       : std_logic;

begin

    uut : entity work.reservation_check_top
        port map (
            origin          => origin,
            intent          => intent,
            occupied_zones  => occupied_zones,
            requested_zones => requested_zones,
            valid_route     => valid_route,
            conflict        => conflict,
            free_slot       => free_slot
        );

    stim_proc : process
    begin

        -- Test 1:
        -- NORTH straight needs CZ3 + CZ1 = 0101.
        -- No zones occupied, therefore free.
        origin         <= "00";
        intent         <= "00";
        occupied_zones <= "0000";
        wait for 10 ns;

        assert requested_zones = "0101"
            report "Test 1 failed: NORTH straight mask incorrect"
            severity error;

        assert conflict = '0'
            report "Test 1 failed: expected no conflict"
            severity error;

        assert free_slot = '1'
            report "Test 1 failed: expected free slot"
            severity error;


        -- Test 2:
        -- NORTH straight needs CZ3 + CZ1 = 0101.
        -- CZ3 is already occupied, therefore conflict.
        origin         <= "00";
        intent         <= "00";
        occupied_zones <= "0100";
        wait for 10 ns;

        assert requested_zones = "0101"
            report "Test 2 failed: NORTH straight mask incorrect"
            severity error;

        assert conflict = '1'
            report "Test 2 failed: expected conflict"
            severity error;

        assert free_slot = '0'
            report "Test 2 failed: expected not free"
            severity error;


        -- Test 3:
        -- SOUTH right needs CZ2 = 0010.
        -- CZ4 is occupied only, therefore no conflict.
        origin         <= "01";
        intent         <= "01";
        occupied_zones <= "1000";
        wait for 10 ns;

        assert requested_zones = "0010"
            report "Test 3 failed: SOUTH right mask incorrect"
            severity error;

        assert conflict = '0'
            report "Test 3 failed: expected no conflict"
            severity error;

        assert free_slot = '1'
            report "Test 3 failed: expected free slot"
            severity error;


        -- Test 4:
        -- EAST right needs CZ4 + CZ3 + CZ1 = 1101.
        -- CZ1 is occupied, therefore conflict.
        origin         <= "10";
        intent         <= "01";
        occupied_zones <= "0001";
        wait for 10 ns;

        assert requested_zones = "1101"
            report "Test 4 failed: EAST right mask incorrect"
            severity error;

        assert conflict = '1'
            report "Test 4 failed: expected conflict"
            severity error;

        assert free_slot = '0'
            report "Test 4 failed: expected not free"
            severity error;


        -- Test 5:
        -- WEST left needs CZ1 = 0001.
        -- CZ2 + CZ3 occupied, therefore no conflict.
        origin         <= "11";
        intent         <= "10";
        occupied_zones <= "0110";
        wait for 10 ns;

        assert requested_zones = "0001"
            report "Test 5 failed: WEST left mask incorrect"
            severity error;

        assert conflict = '0'
            report "Test 5 failed: expected no conflict"
            severity error;

        assert free_slot = '1'
            report "Test 5 failed: expected free slot"
            severity error;


        -- Test 6:
        -- Invalid intent = 11.
        -- Should be invalid, conflict forced high, free forced low.
        origin         <= "00";
        intent         <= "11";
        occupied_zones <= "0000";
        wait for 10 ns;

        assert valid_route = '0'
            report "Test 6 failed: expected invalid route"
            severity error;

        assert conflict = '1'
            report "Test 6 failed: invalid route should be treated as conflict"
            severity error;

        assert free_slot = '0'
            report "Test 6 failed: invalid route should not be free"
            severity error;


        report "All reservation checker tests completed successfully.";
        wait;

    end process;

end sim;
