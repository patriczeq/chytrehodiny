const char file_index_html[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8"/>
    <title>Chytré hodiny</title>
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
                <option value="0">Čas</option>
                <option value="1">Matrix</option>
                <option value="2">Duha</option>
                <option value="3">Snake</option>
             </select>
            </div>
          </div>
       </div>
       <div class="sett-card">
          <h4>Nastavení času</h4>
          <div class="sett-card-content">
             <input type="time" id="manualtime">
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
             <hr>
             <button id="timebutton">Získat z tohoto zařízení</button>
             <button id="ntpbutton">Získat z internetu</button>
             
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
             </div>
          </div>
       </div>
       <div class="sett-card">
          <div class="sett-card-content save">
            <button id="cancel" class="text">Zrušit</button>
            <button id="applyall" class="save">Uložit nastavení</button><br>
          </div>
       </div>
       
       <div class="sett-card">
          <h4>Reset</h4>
          <div class="sett-card-content">
             <div>
                
                <hr>
                <button id="defaults" class="factory">Obnovit výchozí nastavení</button>
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
