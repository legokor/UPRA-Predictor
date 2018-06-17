echo "<?xml version=\"1.0\" encoding=\"UTF-8\"?>
<kml xmlns=\"http://www.opengis.net/kml/2.2\">
<Document>
<name>Predicted Flight Path</name>
<description><![CDATA[Flight data for flight ]]></description>
<Style id=\"bluePoly\">
<LineStyle>
<color>0f00ffff</color>
<width>4</width>
</LineStyle>
<PolyStyle>
<color>7f00ff00</color>
</PolyStyle>
</Style>
<Placemark>
<name>Predicted Flight Path</name>
<description>Flight Data</description>
<styleUrl>#bluePoly</styleUrl>
<LineString>
<extrude>1</extrude>
<tesselate>1</tesselate>
<altitudeMode>absolute</altitudeMode>
<coordinates>
`sed 's/\.[0-9]*$//' $1 | head --lines=-1 | cut --delimiter=',' --complement --fields=1 | awk -F "," '{ print $2 "," $1 "," $3}'`
</coordinates>
</LineString></Placemark>
</Document></kml>"
