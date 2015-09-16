<?php
/**
 * This script is use to monitore virtual rank:
 * 
 * 1. This script verify that we don't have a NULL value into the database for all element
 *    with a tid and a target different from 'none'.
 * 2. This script verify the value to show if we aren"t too higher. Max value is bigint(20).
 * 
 */

/******************************************************************************/
/**********************    Config to send email    ****************************/
/******************************************************************************/
$user = "root";
$pass = "root";
$db_name = "dpi247";
$receiver = 'lba@audaxis.com,jth@audaxis.com';

/******************************************************************************/
/**********************    Script to send email    ****************************/
/******************************************************************************/
$alert_info = "";
$start_msg = "<html>"
              . "<head>"
                . "<title>Rapport d'erreur des virtual rank de DPI</title>"
              . "</head>"
              . "<body>"
                . "<p>Résultats du rapport d'erreur</p>"
                . "<table style='border-collapse: collapse;border: 1px solid black; padding : 5px;'>"
                  . "<tr style='border: 1px solid black; padding : 5px;background-color: #666;color:#FFF;'>"
                    . "<th style='border: 1px solid black; padding : 5px;'>entity_id</th><th style='border: 1px solid black; padding : 5px;'>field_destinations_tid</th><th style='border: 1px solid black; padding : 5px;'>field_destinations_target</th><th style='border: 1px solid black; padding : 5px;'>field_destinations_virtual_rank</th>"
                  . "</tr>";
$end_msg =        "</table>"
              . "</body>"
            . "</html>";

/* Open connexion to database */
$dbh = new PDO('mysql:host=localhost;dbname='.$db_name, $user, $pass);
$query = "SELECT entity_id, field_destinations_tid, field_destinations_target, field_destinations_virtual_rank
          FROM field_data_field_destinations
          WHERE (field_destinations_virtual_rank IS NULL AND field_destinations_target<>'none')
          OR (field_destinations_virtual_rank>1000000000 AND field_destinations_target<>'none')";

foreach($dbh->query($query) as $row) {
  /* Create alert to be send */  
  $alert_info .= "<tr style='border: 1px solid black; padding : 5px;'>
                  <td style='border: 1px solid black; padding : 5px;'>".$row["entity_id"]."</td><td style='border: 1px solid black; padding : 5px;'>".$row["field_destinations_tid"]."</td><td style='border: 1px solid black; padding : 5px;'>".$row["field_destinations_target"].(($row["field_destinations_virtual_rank"]==NULL)? "<td style='border: 1px solid black; padding : 5px;background-color: red;'>NULL</td>" : "<td style='border: 1px solid black; padding : 5px;background-color: orange;'>".$row["field_destinations_virtual_rank"])."</td>"."</td>
                 </tr>";
}

if($alert_info != ""){
  $headers  = 'MIME-Version: 1.0' . "\r\n";
  $headers .= 'Content-type: text/html; charset=iso-8859-1' . "\r\n";
  /* Send email to the receiver */
    mail($receiver, "[DPI-ALERT] Virtual rank problem", $start_msg.$alert_info.$end_msg, $headers);
}
