
library ieee;
use ieee.std_logic_1164.all;

entity route_to_cz_lut is
    port (
        origin      : in  std_logic_vector(1 downto 0);
        intent      : in  std_logic_vector(1 downto 0);
        cz_mask     : out std_logic_vector(3 downto 0);
        valid_route : out std_logic
    );
end route_to_cz_lut;

architecture rtl of route_to_cz_lut is
begin

    process(origin, intent)
    begin
        valid_route <= '1';
        cz_mask     <= "0000";

        case origin is

            -- origin = NORTH
            -- 00 = STRAIGHT: CZ3 -> CZ1
            -- 01 = RIGHT:    CZ3
            -- 10 = LEFT:     CZ3 -> CZ1 -> CZ2
            when "00" =>
                case intent is
                    when "00" =>
                        cz_mask <= "0101"; -- CZ3 + CZ1
                    when "01" =>
                        cz_mask <= "0100"; -- CZ3
                    when "10" =>
                        cz_mask <= "0111"; -- CZ3 + CZ1 + CZ2
                    when others =>
                        cz_mask     <= "0000";
                        valid_route <= '0';
                end case;

            -- origin = SOUTH
            -- 00 = STRAIGHT: CZ2 -> CZ4
            -- 01 = RIGHT:    CZ2
            -- 10 = LEFT:     CZ2 -> CZ4 -> CZ3
            when "01" =>
                case intent is
                    when "00" =>
                        cz_mask <= "1010"; -- CZ2 + CZ4
                    when "01" =>
                        cz_mask <= "0010"; -- CZ2
                    when "10" =>
                        cz_mask <= "1110"; -- CZ2 + CZ4 + CZ3
                    when others =>
                        cz_mask     <= "0000";
                        valid_route <= '0';
                end case;

            -- origin = EAST
            -- 00 = STRAIGHT: CZ4 -> CZ3
            -- 01 = RIGHT:    CZ4 -> CZ3 -> CZ1
            -- 10 = LEFT:     CZ4
            when "10" =>
                case intent is
                    when "00" =>
                        cz_mask <= "1100"; -- CZ4 + CZ3
                    when "01" =>
                        cz_mask <= "1101"; -- CZ4 + CZ3 + CZ1
                    when "10" =>
                        cz_mask <= "1000"; -- CZ4
                    when others =>
                        cz_mask     <= "0000";
                        valid_route <= '0';
                end case;

            -- origin = WEST
            -- 00 = STRAIGHT: CZ1 -> CZ2
            -- 01 = RIGHT:    CZ1 -> CZ2 -> CZ4
            -- 10 = LEFT:     CZ1
            when "11" =>
                case intent is
                    when "00" =>
                        cz_mask <= "0011"; -- CZ1 + CZ2
                    when "01" =>
                        cz_mask <= "1011"; -- CZ1 + CZ2 + CZ4
                    when "10" =>
                        cz_mask <= "0001"; -- CZ1
                    when others =>
                        cz_mask     <= "0000";
                        valid_route <= '0';
                end case;

            when others =>
                cz_mask     <= "0000";
                valid_route <= '0';

        end case;
    end process;

end rtl;