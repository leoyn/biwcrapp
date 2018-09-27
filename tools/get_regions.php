<?php
    $content = file_get_contents("https://www.biwapp.de/");
    $doc = new DomDocument();
    @$doc->loadHTML($content);
    $elements = $doc->getElementsByTagName("option");
    
    $json = array();

    for($i = 0; $i < $elements->length; $i++) {
        $json[$i] = array("name" => $elements[$i]->nodeValue, "abbreviation" => $elements[$i]->getAttribute("value"));
    } 

    // Change the path to your likings
    file_put_contents("../regions.json", json_encode($json));
?>