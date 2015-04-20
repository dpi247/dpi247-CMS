/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


window.addEventListener("keyup", myScript, true);

function myScript(e){
    console.log(e.keyCode);
     if(e.keyCode == 37){         
         var a = document.getElementById('dpi-prev-a').click();
     } else if (e.keyCode == 39) { // right
         var a = document.getElementById('dpi-next-a').click();
     }
}


