/**
 * Replace selectbox by a toggle button
 */
function insertButton(id) {
  var s = document.getElementById(id);

  var button = document.createElement('input');
  button.id = "button-"+id;
  button.type = 'button';
  button.value = s.options[s.selectedIndex].label;
  button.className = "buttonToggle";
  button.onclick = function(e){
    var nextindex = (s.selectedIndex + 1) % 3;
    s.selectedIndex = nextindex;
    this.value = s.options[nextindex].label;
    this.parentNode.className = s.options[nextindex].value;
    var dyndiv = document.getElementById("dynamicmessage");
    if(!dyndiv) {
      var div = document.createElement('div');
      div.id = "dynamicmessage";
      div.className = "messages warning";
      div.innerHTML = "Don't forget to save changes!";
      var content = document.getElementById("edit-submit");
      content.parentNode.insertBefore(div,content);
    }
  };
  s.parentNode.insertBefore(button,s);
  button.parentNode.className = s.options[s.selectedIndex].value;
  s.style.display = "none";
}