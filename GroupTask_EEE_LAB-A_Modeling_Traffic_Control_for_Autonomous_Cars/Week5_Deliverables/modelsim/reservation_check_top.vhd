library ieee;
use ieee.std_logic_1164.all;

entity reservation_check_top is
    port (
        origin         : in  std_logic_vector(1 downto 0);
        intent         : in  std_logic_vector(1 downto 0);
        occupied_zones : in  std_logic_vector(3 downto 0);

        requested_zones : out std_logic_vector(3 downto 0);
        valid_route     : out std_logic;
        conflict        : out std_logic;
        free_slot       : out std_logic
    );
end reservation_check_top;

architecture structural of reservation_check_top is

    signal route_mask : std_logic_vector(3 downto 0);
    signal route_valid : std_logic;
    signal raw_conflict : std_logic;
    signal raw_free : std_logic;

begin

    route_lut_inst : entity work.route_to_cz_lut
        port map (
            origin      => origin,
            intent      => intent,
            cz_mask     => route_mask,
            valid_route => route_valid
        );

    conflict_checker_inst : entity work.conflict_checker
        port map (
            requested_zones => route_mask,
            occupied_zones  => occupied_zones,
            conflict        => raw_conflict,
            free_slot       => raw_free
        );

    requested_zones <= route_mask;
    valid_route     <= route_valid;

    conflict <= raw_conflict when route_valid = '1' else '1';
    free_slot <= raw_free when route_valid = '1' else '0';

end structural;
