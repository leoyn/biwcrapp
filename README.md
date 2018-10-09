
![cc0](https://licensebuttons.net/p/zero/1.0/80x15.png) No rights reserved

Diese Repo soll es erm&ouml;glichen Informationen &uuml;ber lokale Warnmeldungen etc. ohne der BIWAPP abrufen zu k&ouml;nnen.


## Der Grund f&uuml;r diese Repo?

1. Die Android-App sowie iOS-App sind beide nicht wirklich zu gebrauchen (siehe Nutzerreviews).
2. Properit&auml;re Software, die mehr und mehr von Landkreisen genutzt wird und die Freiheit der B&uuml;rger einschr&auml;nkt.
3. Anregung zur Besserung der aktuellen Lage.

## Zugriff mit curl

Eine Liste aller Lokationen sind [hier](#Landkreise%20und%20Regionen) zu finden.

```
location={insertLocationHere}
curl -H "Content-Type: application/x-www-form-urlencoded" --data "location=$location" "https://www.biwapp.de/widget/dataBiwappProxy"
```

# Landkreise und Regionen

| Abk&uuml;rzung| |
|--------------|--------------|
|`calw`|Calw (Landkreis)|
|`cloppenburg`|Cloppenburg (Landkreis)|
|`cuxhaven (stadt)`|Cuxhaven (Stadt)|
|`cuxhaven (landkreis)`|Cuxhaven (Landkreis)|
|`darmstadt`|Darmstadt (Stadt)|
|`delmenhorst`|Delmenhorst (Stadt)|
|`dingolfing-landau`|Dingolfing-Landau (Landkreis)|
|`freudenstadt`|Freudenstadt|
|`landkreis görlitz`|G&ouml;rlitz (Landkreis)|
|`günzburg`|G&uuml;nzburg (Landkreis)|
|`gütersloh`|G&uuml;tersloh (Landkreis)|
|`harburg (landkreis)`|Harburg (Landkreis)|
|`heidekreis`|Heidekreis (Landkreis)|
|`hildesheim (landkreis)`|Hildesheim (Landkreis)|
|`hildesheim (stadt)`|Hildesheim (Stadt)|
|`hochtaunuskreis`|Hochtaunuskreis (Landkreis)|
|`landshut (landkreis)`|Landshut (Landkreis)|
|`landshut (stadt)`|Landshut (Stadt)|
|`lüneburg`|L&uuml;neburg (Landkreis)|
|`lüchow-dannenberg`|L&uuml;chow-Dannenberg (Landkreis)|
|`mansfeld-suedharz`|Mansfeld-S&uuml;dharz (Landkreis)|
|`märkisch-oderland`|M&auml;rkisch Oderland (Landkreis)|
|`meißen`|Mei&szlig;en (Landkreis)|
|`mittelsachsen`|Mittelsachsen (Landkreis)|
|`mühldorf`|M&uuml;hldorf am Inn (Landkreis)|
|`norderstedt (stadt)`|Norderstedt (Stadt)|
|`northeim`|Northeim (Landkreis)|
|`oldenburg`|Oldenburg (Landkreis)|
|`passau`|Passau (Landkreis)|
|`peine`|Peine (Landkreis)|
|`rostock städte`|Rostock (Stadt)|
|`rottal-inn`|Rottal-Inn (Landkreis)|
|`Rottweil`|Rottweil (Landkreis)|
|`sächsische schweiz-osterzgebirge`|S&auml;chsische Schweiz-Osterzgebirge (Landkreis)|
|`schaumburg`|Schaumburg (Landkreis)|
|`vechta`|Vechta (Landkreis)|
|`verden`|Verden (Landkreis)|
|`wilhelmshaven`|Wilhelmshaven (Stadt)|

-----

## Installation der tools

```
./configure
make
```

