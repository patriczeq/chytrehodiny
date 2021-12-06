const char file_picker_js[] PROGMEM = R"=====(
/*
 * Simple hsl color picker
 *
 *  Patrik Eder 2021
 *  patrikeder.cz
 *  
 *  <input type="color" id="example">
 *  ...
 *  function onInputHandler(color){
 *    console.log(color);
 *  }
 *  new myPicker("#example", onChangeHandler(color), onInputHandler(color), true);
 *  
*/
class myPicker{
        constructor(id = "", onchange = null, oninput = null, hideOriginal = true){
          this.hsl = [180,100,50];
          this.id = id;
          this.hide = hideOriginal;
          this.onchange = onchange === null ? function(color){} : onchange;
          this.oninput = oninput === null ? function(color){} : oninput;
          this.parent = document.getElementById(this.id).parentElement;
          this.defaultstyle = `

.my-picker-wrapper{
  display: block;
  overflow: hidden;
}
.my-picker-range,
.my-picker-light,
.my-picker-saturate{
  -webkit-appearance: none;
  width: 100%;
  outline: none;
  overflow: visible;
  height: 32px;
  background-size: 100% 100%;
  background-repeat: no-repeat;
  border: 1px solid #ddd;
  border-radius: 4px;
  padding: 0;
}
.my-picker-range{
  background-image: linear-gradient(90deg, red, yellow, lime, cyan, blue, magenta, red);
}
.my-picker-saturate{
  background-image: linear-gradient(90deg, #737373, transparent);
}
.my-picker-light{
  background-image:linear-gradient(90deg, black, transparent, white);
}
.my-picker-range::-moz-range-thumb,
.my-picker-saturate::-moz-range-thumb,
.my-picker-light::-moz-range-thumb,
.my-picker-range::-webkit-slider-thumb,
.my-picker-saturate::-webkit-slider-thumb,
.my-picker-light::-webkit-slider-thumb{
    width: 12px;
    height: 100%;
    background: transparent;
    border: 3px solid #fff;
    cursor: pointer;
    border-radius: 8px;
    border: 0;
    box-shadow: 4px 4px 8px rgba(0, 0, 0, .25), inset 0 0 8px rgba(0, 0, 0, .25);
}`;
          this.dom();
          this.readInput();
          this.hex = document.getElementById(this.id).value;
          this.listen();
          this.updated = 0;
        }
        readInput(){
          this.hex = document.getElementById(this.id).value;
          let val = this.hexToHsl(this.hex);
          this.hsl[0] = val.h;
          this.hsl[1] = val.s;
          this.hsl[2] = val.l;
          document.getElementById(`${this.id}_range`).value = this.hsl[0];
          document.getElementById(`${this.id}_saturate`).value = this.hsl[1];
          document.getElementById(`${this.id}_light`).value = this.hsl[2];
          document.getElementById(`${this.id}_light`).style['background-color'] = `hsl(${this.hsl[0]}, 100%, 50%)`;
          document.getElementById(`${this.id}_saturate`).style['background-color'] = `hsl(${this.hsl[0]}, 100%, 50%)`;
        }
        dom(){
          let style = document.createElement("style"),
              wrapper = document.createElement("div");
              wrapper.classList.add("my-picker-wrapper");
              style.innerHTML = this.defaultstyle;
              document.querySelector("head").prepend(style);
              
          let range = document.createElement('input'),
              light = document.createElement('input'),
              saturate = document.createElement('input');
              
              range.setAttribute("type", "range");
              light.setAttribute("type", "range");
              saturate.setAttribute("type", "range");
              
              range.setAttribute("min", "0.01");
              range.setAttribute("max", "359.99");
              light.setAttribute("min", "0.01");
              light.setAttribute("max", "99.99");
              saturate.setAttribute("min", "0.01");
              saturate.setAttribute("max", "99.99");

              range.setAttribute("step", "0.01");
              light.setAttribute("step", "0.01");
              saturate.setAttribute("step", "0.01");
              
              range.value = this.hsl[0];
              saturate.value = this.hsl[1];
              light.value = this.hsl[2];
              
              range.setAttribute("id", `${this.id}_range`);
              light.setAttribute("id", `${this.id}_light`);
              saturate.setAttribute("id", `${this.id}_saturate`);
              
              range.classList.add("my-picker-range");
              light.classList.add("my-picker-light");
              saturate.classList.add("my-picker-saturate");
              
              light.style['background-color'] = `hsl(${this.hsl[0]}, 100%, 50%)`;
              saturate.style['background-color'] = `hsl(${this.hsl[0]}, 100%, 50%)`;
              
            wrapper.appendChild(range);
            wrapper.appendChild(saturate);
            wrapper.appendChild(light);
            this.parent.appendChild(wrapper);
            if(this.hide){
              document.getElementById(this.id).style.display = "none";
            }
        }
        listen(){
          let that = this;
          document.getElementById(`${this.id}_range`).addEventListener("input", e => { 
            that.hsl[0] = e.target.value;
            document.getElementById(`${this.id}_light`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            document.getElementById(`${this.id}_saturate`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            that.oninput(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
            that.apply();
          });
          document.getElementById(`${this.id}_light`).addEventListener("input", e => { 
            that.hsl[2] = e.target.value;
            document.getElementById(`${this.id}_light`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            document.getElementById(`${this.id}_saturate`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            that.oninput(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
            that.apply();
          });
          document.getElementById(`${this.id}_saturate`).addEventListener("input", e => { 
            that.hsl[1] = e.target.value;
            document.getElementById(`${this.id}_light`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            document.getElementById(`${this.id}_saturate`).style['background-color'] = `hsl(${that.hsl[0]}, 100%, 50%)`;
            that.oninput(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
            that.apply();
          });
          document.getElementById(this.id).addEventListener("input", e => { 
            that.readInput();
          });
          
          document.getElementById(`${this.id}_range`).addEventListener("change", e => {
            that.onchange(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
          });
          document.getElementById(`${this.id}_saturate`).addEventListener("change", e => {
            that.onchange(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
          });
          document.getElementById(`${this.id}_light`).addEventListener("change", e => {
            that.onchange(that.hslToHex(that.hsl[0], that.hsl[1], that.hsl[2]));
          });
        }
        apply(){
          document.getElementById(this.id).value = this.hslToHex(this.hsl[0], this.hsl[1], this.hsl[2]);
          this.updated = Number(new Date());
        }
        hslToHex(h, s, l) {
          l /= 100;
          const a = s * Math.min(l, 1 - l) / 100;
          const f = n => {
            const k = (n + h / 30) % 12;
            const color = l - a * Math.max(Math.min(k - 3, 9 - k, 1), -1);
            return Math.round(255 * color).toString(16).padStart(2, '0');   // convert to Hex and prefix "0" if needed
          };
          return `#${f(0)}${f(8)}${f(4)}`;
        }
        hexToHsl(hex) {
          let result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex),
            r = parseInt(result[1], 16),
            g = parseInt(result[2], 16),
            b = parseInt(result[3], 16);
            r /= 255, g /= 255, b /= 255;
            var max = Math.max(r, g, b), min = Math.min(r, g, b);
            var h, s, l = (max + min) / 2;
            if(max == min){
              h = s = 0; // achromatic
            }else{
              var d = max - min;
              s = l > 0.5 ? d / (2 - max - min) : d / (max + min);
              switch(max){
                case r: h = (g - b) / d + (g < b ? 6 : 0); break;
                case g: h = (b - r) / d + 2; break;
                case b: h = (r - g) / d + 4; break;
              }
              h /= 6;
            }
          var HSL = new Object();
          HSL['h']=h*360;
          HSL['s']=s*100;
          HSL['l']=l*100;
          return HSL;
        }
      }
)=====";
