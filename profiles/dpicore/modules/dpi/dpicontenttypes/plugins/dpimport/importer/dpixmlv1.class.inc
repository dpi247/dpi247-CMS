<?php 

class DpiXmlV1 extends Importer{
  function process_packages($dom){
    dsm($dom->nodeName);
    print $dom->nodeName;
  }
  function process_rss($dom){
    dsm($dom->nodeName);
  }
}