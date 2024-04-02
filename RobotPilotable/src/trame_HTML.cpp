
// HTML pour le joystick virtuel
const char *html_joystick = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Joystick Virtuel</title>
  <style>
    /* CSS pour le joystick */
    :root {
      --joystick-size: 500px; /* Variable pour la taille du joystick */
      --thumb-size: 100px; /* Variable pour la taille du pouce du joystick */
    }

    #joystick {
      position: relative;
      width: var(--joystick-size);
      height: var(--joystick-size);
      background-color: #ccc;
      border-radius: 50%;
      touch-action: none; /* Désactiver le défilement lors du déplacement du joystick */
    }

    #thumb {
      position: absolute;
      width: var(--thumb-size);
      height: var(--thumb-size);
      background-color: #f00;
      border-radius: 50%; /* Pour rendre le pouce rond */
      top: calc(50% - var(--thumb-size) / 2); /* Placer le pouce au milieu verticalement */
      left: calc(50% - var(--thumb-size) / 2); /* Placer le pouce au milieu horizontalement */
    }

    #coordinates {
      margin-top: 20px;
      font-size: 16px;
    }
  </style>
</head>
<body>
  <h2>Joystick Virtuel</h2>
  <div id="joystick">
    <div id="thumb"></div>
  </div>
  <div id="coordinates">Coordonnées: X <span id="xCoord">0</span>, Y <span id="yCoord">0</span></div>
  <script>
    // JavaScript pour le joystick virtuel
    var joystick = document.getElementById('joystick');
    var thumb = document.getElementById('thumb');
    var joystickX = 0;
    var joystickY = 0;
    var isDragging = false; // Variable pour suivre si l'utilisateur est en train de glisser le pouce

    // Récupérer la taille du joystick depuis les variables CSS
    const joystickSize = parseInt(getComputedStyle(joystick).getPropertyValue('--joystick-size'));
    const thumbSize = parseInt(getComputedStyle(thumb).getPropertyValue('--thumb-size'));
    const ZeroX = joystickSize / 2 - thumbSize / 2;
    const ZeroY = joystickSize / 2 - thumbSize / 2;

    function updateCoordinates(event) {
      if (isDragging) {
        // Mettre à jour les coordonnées du joystick uniquement si l'utilisateur est en train de glisser

        joystickX = event.touches[0].clientX - joystick.offsetLeft - thumbSize / 2 ;
        joystickY = event.touches[0].clientY - joystick.offsetTop - thumbSize / 2;

        // Limite à ne pas dépasser
        joystickX = Math.max(-thumbSize / 2, Math.min(joystickX, joystickSize - thumbSize / 2));
        joystickY = Math.max(-thumbSize / 2, Math.min(joystickY, joystickSize - thumbSize / 2));
        // Positionnement
        thumb.style.left = joystickX + 'px';
        thumb.style.top = joystickY + 'px';


        // Calcul des x,y, max 100, min -100 

        var TrueX = ((joystickX - ZeroX) * 200) / joystickSize;
        var TrueY = -((joystickY - ZeroX) * 200) / joystickSize;

        // Mise à jour des coordonnées affichées
        document.getElementById('xCoord').innerText = TrueX.toFixed(0);
        document.getElementById('yCoord').innerText = TrueY.toFixed(0);

          // Affichage des valeurs dans la console pour débug
  /*      console.log("joystickX :", joystickX);  
        console.log("thumb.style.left :", thumb.style.left);
        console.log("joystick X = ", event.clientX, " - ", joystick.offsetLeft, " - ", thumbSize, " / 2"); */
		  
		// Envoi des coordonnées trueX et TrueY toute les x ms
		  
	    setInterval(sendCoordinates(TrueX.toFixed(0),TrueY.toFixed(0)), 500);
      }
    }

thumb.addEventListener('touchstart', function(event) {
    // Lorsque l'utilisateur touche le pouce, activer le suivi du glissement
    isDragging = true;
    // Démarrer l'envoi des coordonnées à intervalles réguliers
    intervalID = setInterval(function() {
        sendCoordinates(TrueX.toFixed(0), TrueY.toFixed(0));
    }, 500); // Envoyer les coordonnées toutes les 500 ms
});

    document.addEventListener('touchmove', function(event) {
      // Si l'utilisateur est en train de glisser le pouce, mettre à jour les coordonnées
      if (isDragging) {
        updateCoordinates(event);
        
      }
    });

    document.addEventListener('touchend', function(event) {
      // Lorsque le doigt quitte l'écran, désactiver le suivi du glissement
      isDragging = false;
clearInterval(intervalID);
        thumb.style.left = ZeroX + 'px';
        thumb.style.top = ZeroY + 'px';


      // Mettre à jour les coordonnées affichées
        document.getElementById('xCoord').innerText = 0;
        document.getElementById('yCoord').innerText = 0;
		
		
		sendCoordinates(0,0);

    });
function sendCoordinates(x, y) {
  var xhr = new XMLHttpRequest();
  xhr.open("POST", "/update_coordinates", true);
  xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
  xhr.onreadystatechange = function() {
    if (xhr.readyState === XMLHttpRequest.DONE && xhr.status === 200) {
      // Réponse reçue du serveur
      console.log("Coordonnées envoyées avec succès !");
    }
  };
  var data = "x=" + encodeURIComponent(x) + "&y=" + encodeURIComponent(y);
  xhr.send(data);
}
	  
  </script>
</body>
</html>

)rawliteral";
