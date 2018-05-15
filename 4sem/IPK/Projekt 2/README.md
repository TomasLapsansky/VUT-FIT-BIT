Aplikacia realizuje jednoduchy DHCP Starvation attack pomocou vycerpania DHCP poolu DISCOVERY spravami. Blizsie informacie o implementacii a moznych chybas viz. dokumentace.pdf

Kompilacia:
	make all

Spustenie:
	./ipk-dhcpstarve -i <interface>
	<interface> - meno interfacu z ifconfig
