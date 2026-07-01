library ieee;
use ieee.std_logic_1164.all;

entity conflict_checker is
    port (
        requested_zones : in  std_logic_vector(3 downto 0);
        occupied_zones  : in  std_logic_vector(3 downto 0);
        conflict        : out std_logic;
        free_slot       : out std_logic
    );
end conflict_checker;

architecture rtl of conflict_checker is
    signal overlap : std_logic_vector(3 downto 0);
    signal any_overlap : std_logic;
begin

    overlap <= requested_zones and occupied_zones;

    any_overlap <= overlap(0) or overlap(1) or overlap(2) or overlap(3);

    conflict  <= any_overlap;
    free_slot <= not any_overlap;

end rtl;