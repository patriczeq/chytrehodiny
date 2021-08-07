const char file_index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8"/>
    <title>Hodiny</title>
    <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
    <meta name="viewport" content="width=device-width,initial-scale=1">
    <meta name="description" content="Hodiny">
    <meta name="author" content="Patrik EDER">
    <link rel="icon" type="image/x-icon" href="favicon.ico">
    <link rel="icon" type="image/png" sizes="256x256" href="favicon256.png">
    <link rel="manifest" href="site.webmanifest">
    <meta name="apple-mobile-web-app-title" content="Hodiny">
    <meta name="application-name" content="Hodiny">
    <meta name="msapplication-TileColor" content="#eee">
    <meta name="theme-color" content="#eee">
    <link href="style.css" rel='stylesheet' type='text/css'>
    <meta name="apple-mobile-web-app-capable" content="yes">
    <meta name="apple-mobile-web-app-status-bar-style" content="black">
    <link href="style.css" rel='stylesheet' type='text/css'>
  </head>
  <body>
    <div id="clock"></div>
    <div class="nastaveni">
       <div class="sett-card">
          <h4>Zobrazení</h4>
          <div class="sett-card-content">
             <div>
              <label for="colorselect">Výchozí barva</label>
              <input type="color" id="colorselect"><br>
              <label for="bgcolorselect">Barva pozadí</label>
              <input type="color" id="bgcolorselect"><br>
              <label for="modeselect">Mód zobrazení</label>
              <select id="modeselect">
                <option value="1">Čas</option>
                <option value="2">Matrix</option>
                <option value="3">Duha</option>
                <option value="4">Had</option>
             </select>
             <br>
             <label for="bright">Jas:</label>
             <input type="range" min="0" max="255" id="bright">
            </div>
          </div>
       </div>
       <div class="sett-card">
          <h4>Nastavení času</h4>
          <div class="sett-card-content">
             <input type="time" id="manualtime">
             <hr>
             <button id="timebutton">Získat z tohoto zařízení</button><br>
             <button id="ntpbutton">Získat z internetu</button>
             <select id="timezone">
                <option value="0">0 GMT</option>
                <option value="1">+1 GMT</option>
                <option value="2">+2 GMT</option>
                <option value="3">+3 GMT</option>
                <option value="4">+4 GMT</option>
                <option value="5">+5 GMT</option>
                <option value="6">+6 GMT</option>
                <option value="7">+7 GMT</option>
                <option value="8">+8 GMT</option>
                <option value="9">+9 GMT</option>
                <option value="10">+10 GMT</option>
                <option value="11">+11 GMT</option>
             </select>
             
          </div>
       </div>

       <div class="sett-card">
          <h4>Připojení k wifi</h4>
          <div class="sett-card-content">
             <div>
                <span id="status">...</span>
                <hr>
                <button id="scanbutton">Vyhledat sítě</button>
                <select id="networks" style="width: 100%;"></select>
                <input type="password" style="width: 100%;" placeholder="heslo" id="pwd">
                <button id="connect">Připojit</button>
                <hr>
                <i id="afterconnect">Po úspěšném připojení k síti bude zažízení dostupné na adrese <a href="http://hodiny.local">hodiny.local</a></i>
             </div>
          </div>
       </div>
       <div class="sett-card">
          <div class="sett-card-content save">
            <button id="cancel" class="text">Zpět</button>
            <button id="applyall" class="save">Uložit</button>
          </div>
       </div>
       
       <div class="sett-card">
          <h4>Reset</h4>
          <div class="sett-card-content">
             <div>
                
                <hr>
                <button id="defaults" class="factory">Tovární nastavení</button>
             </div>
          </div>
      </div>
    </div>

    <script type="module">
      import Clock from './clock.js';
      window.hodiny = new Clock("#clock");
    </script>
  </body>
</html>
)=====";
