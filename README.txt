Aplikacia pre zobrazenie liniek hromadnej dopravy a ich pohybu na mape:

 *	Autor: Miroslav Balaz <xbalaz09@stud.fit.vutbr.cz>
 *	Autor: Andrej Pavlovic <xpavlo.stud.fit.vutbr.cz>

Data po spusteni aplikacie spracuje data holder.

1 sekunda v aplikacii pri zakladnej rychlosti znamena 1 sekundu realneho casu.
Dlzky ciest nahravanych zo suboru udavaju ich casovu vzdialenost v sekundach.

Ztazenie prevadzky:		- oranzova cesta < 10s, cervena cesta >= 10s.

Uzatvorenie cesty:		- je mozne uzavriet viacero usekov, nie iba jeden,
						- je mozne uzavriet lubovolny pocet ciest a lubovolny pocet obchadzkovych ciest v jednom precese uzatvarania,
						- ak si uzivatel rozmysli uzatvaranie ciest pocas volenia ciest, je mozno vyber zrusit a odist z modu uzatvarania ciest kliknutim mimo cesty,
						- je mozne zvolit lubovolny pocet ciest pri jednom procese uzatvarania,
						- zvlona uzavreta cesta musi navazovat na predoslu uzavretu,
						- zvlona obchadzkova cesta musi navazovat na predoslu obchadzkovu,
						- uzavrety usek sa da zrusit a uviest do povodneho stavu pomocou kliknutim na jednu z uzatvorenych ciest trasy a kliknutim na tlacidlo zrusenia.