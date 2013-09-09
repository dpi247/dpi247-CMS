<?php 

class DpiXmlV1 extends Importer{
  function build_packages($dom){
    dsm($dom->nodeName);
    print $dom->nodeName;
  }
}