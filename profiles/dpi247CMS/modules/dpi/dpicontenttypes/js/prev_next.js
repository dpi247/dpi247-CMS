/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */


window.addEventListener("keyup", prevNextKeyPress, true);

function prevNextKeyPress(e){
    if(e.keyCode == 37){         
        document.getElementById('dpi-prev-a').click();
    } else if (e.keyCode == 39) {
        document.getElementById('dpi-next-a').click();
    }
}


