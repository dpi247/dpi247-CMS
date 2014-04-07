<?php 

class DpiXmlV1 extends Importer{
  function build_packages($dom){
    print $dom->nodeName;
  }
}
