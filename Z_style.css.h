const char file_style_css[] PROGMEM = R"=====(







:root {
    --cell-size: calc(65vh / 11);
    --color-info: #2353ae;
    --color-success: #23ae23;
    --color-warning: #ff6c00;
    --color-danger: )#df0f36;
    --color-background: #dddddd;
    --color-background-dark: #222;
    --color-text: #111111;
    --color-text-dark: #eeeeee;
}

html, body {
    padding: 0;
    margin: 0;
    perspective: calc(var(--cell-size) * 11);
    overflow: hidden;
    height: 100vh;
    width: 100vw;
    transition: .2s linear background;
}

body {
    background-color: var(--color-background);
    color: var(--color-text);
    transform-origin: center center;
    overflow-y: auto;
    font-family: Sans-serif;
}
body.loading{
    overflow: hidden;
}
a {
    color: currentColor;
}
.hidden{
    display: none;
}
label{
    cursor: pointer;
    user-select: none;
}

.noarea{
  position: absolute;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  cursor: pointer;
}

#notify{
    position: fixed;
    display: block;
    background-color: #000;
    border-radius: 8px;
    box-shadow: 0 8px 16px rgba(0,0,0,.2);
    min-width: 300px;
    width: 25%;
    position: fixed;
    left: 50%;
    bottom: 16px;
    padding: 8px;
    font-size: 24px;
    text-align: center;
    transform: translateX(-50%);
    color: #fff;
    pointer-events: none;
    opacity: 0;
    z-index: 2;
    transition: .5s ease-in-out opacity;
}
#notify.visible{
    opacity: 1;
}

#notify.info{
    background-color: var(--color-info);
}
#notify.success{
    background-color: var(--color-success);
}
#notify.warning{
    background-color: var(--color-warning);
}
#notify.danger{
    background-color: var(--color-danger);
}

.loading #notify{
    color: #fff;
    background-color: transparent;
    box-shadow: none;
    opacity: 1;
}

.dayinfo{
    font-size: calc(var(--cell-size) / 2);
    font-weight: 200;
    position: absolute;
    bottom: 0;
    left: 0;
    width: 100vw;
    text-align: center;
    height: calc(100vh - calc(var(--cell-size) * 14));
    user-select: none;
    opacity: 1;
    transition: .4s ease-in-out all;
}
.loading .dayinfo{
    color: #fff;
    bottom: 50%;
    transform: translateY(50%);
}

.sett .dayinfo{
    opacity: 0
}

.nastaveni, .hodiny {
    position: relative;
    right: 50%;
    transform: translate(-50%, calc(var(--cell-size) / 4)) rotateY(0deg);
    width: calc(var(--cell-size) * 11);
    background-image: linear-gradient(-45deg, #171717, #0d0d0d);
    padding: calc(var(--cell-size) / 4);
    transition: .5s ease-in-out all;
    transform-origin: center;
    transform-box: border-box;
    transform-style: preserve-3d;
}

.hodiny {
    cursor: pointer;
    position: fixed;
    right: 50vw;
    top: var(--cell-size);
    transform: translate(50%, 0%);
    box-shadow: 8px 8px 32px rgba(0,0,0,.5), 0 0 0 calc(var(--cell-size) / 4) rgba(255,255,255,.5);
    border-radius: 8px;
    background-color: #171717;
    filter: brightness(2) blur(.75px);
    transition: .4s ease-in-out filter, .4s ease-in-out transform;
}

.loading .hodiny{
    filter: brightness(1) blur(0px);
    transform: translate(50%, 0%) scale(4);
}

#clock {
    transform-style: preserve-3d;
}
.hodiny svg{
    position: absolute;
}
#boardmask{
    -webkit-transform: scale(0.002,0.002) translate(-500%,-1000%);
    transform: scale(0.002,0.002) translate(-500%,-1000%);
}
.hodiny .pixels,
.hodiny .pixels canvas{
    width: 100%;
    height: 100%;
}

.hodiny .pixels{
    clip-path: url(#boardmask);
   -webkit-clip-path: url(#boardmask);
    background-color: #000;
}

.sett .hodiny {
    transform: translate(0%, 0%) scale(.75);
    right: 50vw;
}

.nastaveni {
    background-color: #eee;
    background-image: none;
    top: 0;
    right: 0;
    position: absolute;
    z-index: 0;
    transform: translate(100%, 0%);
    pointer-events: none;
    box-shadow: none;
    color: #000;
    overflow: auto;
    border-radius: calc(var(--cell-size) / 8);
    font-size: 16px;
    height: calc(100% - 32px);
    overflow: hidden;
    border-radius: 0;
    max-width: 50%;
    width: 640px;
}

.sett .nastaveni {
    transform: translate(0%, 0%) scale(1);
    pointer-events: all;
}

.nastaveni .sett-header, .nastaveni::after {
    font-size: 24px;
    font-weight: 200;
    padding: 8px;
    height: 48px;
    z-index: 2;
    position: relative;
    box-shadow: 0 16px 16px #eee;
    line-height: 3;
}

.nastaveni::after {
    content: "";
    display: inline-block;
    position: absolute;
    bottom: 0;
    left: 0;
    width: 100%;
    background-color: #eee;
    height: 16px;
    padding: 0;
    box-shadow: 0 -8px 8px #eee;
}

.nastaveni .sett-header button {
    border: 0;
    background: transparent;
    width: 48px;
    padding: 0;
    margin: 0;
    display: inline-block;
    min-width: 48px;
    position: relative;
    padding-left: 48px;
    font-size: 24px;
    width: 100%;
    text-align: left !important;
}

.nastaveni .sett-header button#cancel::before, .nastaveni .sett-header button#cancel::after {
    content: "";
    width: 16px;
    height: 1px;
    background-color: #333;
    transform: rotate(45deg) translate(50%, -50%);
    position: absolute;
    left: 8px;
    top: 50%;
}

.nastaveni .sett-header button#cancel::after {
    transform: rotate(-45deg) translate(50%, -50%);
}

.nastaveni .sett-content {
    height: calc(100% - 256px);
    overflow: auto;
    padding: 16px 8px 128px 8px;
    position: absolute;
    right: 0;
    width: calc(100% - 32px);
}

.nastaveni #colorselect {
}

.nastaveni .sett-card {
    box-shadow: -8px -8px 16px rgba(255, 255, 255, .25), 8px 8px 8px rgba(0, 0, 0, .05);
    padding: 16px;
    border-radius: 8px;
    background-color: #fff;
    margin-bottom: 16px;
    position: relative
}

.nastaveni .sett-card h4 {
    padding: 0;
    margin: 0;
    margin-left: 32px;
    margin-bottom: 8px;
    font-size: 24px;
    font-weight: 400;
    color: #111;
    position: relative;
}
.nastaveni .sett-card .icon{
    width: 26px;
    height: 26px;
    position: absolute;
    left: 12px;
    top: 12px;
    opacity: .75;
    filter: drop-shadow(0 0 2px);
}
.nastaveni .sett-card .icon path,
.nastaveni .sett-card .icon polygon,
.nastaveni .sett-card .icon circle{
    fill: currentColor;
}


.nastaveni .sett-card .sett-card-content {
    padding: 8px;
    font-size: 16px;
}

.nastaveni .sett-card .sett-card-actions {
    border-top: 1px solid #ddd;
    display: flex;
    flex-flow: row;
    padding: 8px;
}

.nastaveni label {
    margin-right: 8px;
}

.nastaveni input, .nastaveni button, .nastaveni select, .nastaveni input:focus, label.dropdownlabel {
    font-size: 16px;
    border: 0;
    color: #000;
    background-color: rgba(0, 0, 0, .05);
    padding: 4px 8px 4px 8px;
    outline: none;
    border-radius: 16px;
    text-align: center;
    min-width: 150px;
    margin: 4px;
    border: 1px solid rgba(0, 0, 0, .05);
    color: #333;
    text-align: center !important;
    cursor: pointer;
}

.nastaveni .toggle + label{
    display: inline-block;
    width: 48px;
    height: 24px;
    border: 1px solid rgba(128,128,128, .15);
    background-color: rgba(128,128,128, .15);
    cursor: pointer;
    border-radius: 24px;
    position: relative;
    transition: .2s ease-in-out all;
    margin-bottom: -6px;
    margin-top: 6px;
    position: relative;
    overflow: hidden;
}
.nastaveni .toggle:checked + label{
    background-color: #3fca3f;
    border: 1px solid #0ae30a;
}
.nastaveni .toggle + label::before{
    content: "";
    width: 20px;
    height: 20px;
    background: #fff;
    cursor: pointer;
    border-radius: 50%;
    border: 0;
    box-shadow: 4px 4px 8px rgba(0, 0, 0, .25);
    border-radius: 100%;
    background-color: #fff;
    display: inline-block;
    position: absolute;
    left: 50%;
    top: 50%;
    transform: translate(-100%, -50%);
    transition: .2s ease-in-out all;
}
.nastaveni .toggle:checked + label::before{
    transform: translate(0%, -50%);
}


.nastaveni #autobright:checked ~ input{
    display: none;
}

.nastaveni .sch{
    display: none;
    border: 1px solid rgba(128,128,128,.25);
    padding: 16px;
    border-radius: 8px;
    margin-top: 8px;
}
.nastaveni #schedule:checked ~ .sch{
   display: inline-block
}

.nastaveni input[type="color"] {
    min-width: 24px;
    width: 24px;
    height: 24px;
    padding: 0;
    margin: 0;
}

.nastaveni button, .nastaveni select {
    background-color: transparent;
}

.nastaveni select:empty, .nastaveni select:empty + input, .nastaveni select:empty ~ button {
    display: none;
}

.nastaveni select option {
    background: #fff;
    color: #111;
}

.nastaveni input[type='range'] {
    -webkit-appearance: none;
    width: 100%;
    height: 20px;
    outline: none;
    overflow: hidden;
    padding: 2px;
}

.nastaveni input[type='range']::-webkit-slider-thumb, .nastaveni input[type='range']::-moz-range-thumb {
    width: 20px;
    height: 20px;
    background: #fff;
    cursor: pointer;
    border-radius: 50%;
    border: 0;
    box-shadow: 4px 4px 8px rgba(0, 0, 0, .25)
}

.nastaveni button.factory {
    background-color: transparent;
    color: #e60000;
    border: 1px solid #e60000;
    font-size: 14px;
}

.nastaveni button.save {
    background-color: transparent;
    color: #fff;
    border: 1px solid #00e600;
    background-color: #3fca3f;
}

.nastaveni input[type="password"] {
    text-align: left;
}

.nastaveni hr {
    background-color: #ddd;
    border: 0;
    height: 1px;
    box-shadow: 0 0 8px rgba(0, 0, 0, .25)
}

.nastaveni .sett-card .sett-card-content.save {
    display: flex;
    flex-flow: row;
}

.nastaveni .sett-card .sett-card-content.save button {
    display: flex;
    flex-flow: column;
    flex-grow: 1;
    position: relative;
}

.nastaveni .sett-card .sett-card-content.save button#cancel::before, .nastaveni .sett-card .sett-card-content.save button#cancel::after {
    content: "";
    width: 6px;
    height: 1px;
    background-color: #999;
    transform: rotate(45deg) translate(50%, -50%);
    position: absolute;
    left: 8px;
    top: 50%;
}

.nastaveni .color {
    display: inline-block;
    overflow: hidden;
    border-radius: 50%;
    border: 0;
    outline: none;
    pointer-events: none;
}

.nastaveni .sett-card .sett-card-content.save button#cancel::after {
    transform: rotate(-45deg) translate(50%, -50%);
}

.my-picker-wrapper {
    border: 1px solid rgba(128, 128, 128, .5);
    padding: 8px;
    border-radius: 8px;
}

.my-picker-range, 
.my-picker-saturate, 
.my-picker-light {
    overflow: visible !important;
    height: 32px !important;
    width: calc(100% - 16px) !important;
    background-size: 100% 100% !important;
    background-repeat: no-repeat !important;
    border: 1px solid #ddd !important;
    border-radius: 4px !important;
    padding: 0 !important;
}

.my-picker-range::-moz-range-thumb, 
.my-picker-saturate::-moz-range-thumb, 
.my-picker-light::-moz-range-thumb{
    width: 12px !important;
    height: 40px !important;
    background: transparent !important;
    border: 3px solid #fff !important;
    cursor: pointer;
    border-radius: 8px !important;
    border: 0;
    box-shadow: 4px 4px 8px rgba(0, 0, 0, .25), inset 0 0 8px rgba(0, 0, 0, .25) !important;
}
.my-picker-range::-webkit-slider-thumb, 
.my-picker-saturate::-webkit-slider-thumb, 
.my-picker-light::-webkit-slider-thumb{
    -webkit-appearance: none;
    width: 12px !important;
    height: 40px !important;
    background: transparent !important;
    border: 3px solid #fff !important;
    cursor: pointer;
    border-radius: 8px !important;
    border: 0;
    box-shadow: 4px 4px 8px rgba(0, 0, 0, .25), inset 0 0 8px rgba(0, 0, 0, .25) !important;
}

.dropdownlabel {
    display: inline-block;
    position: relative;
    top: -4px;
    user-select: none;
    cursor: pointer;
}

.dropdowncheck {
    display: none;
}

.dropdowncheck ~ .my-picker-wrapper {
    height: 0;
    padding: 0;
    transition: .2s linear height
}

.dropdowncheck:checked ~ .my-picker-wrapper {
    height: 132px;
}

.dropdowncheck ~ .dropdownlabel::before {
    content: "";
    width: 12px;
    height: 12px;
    border-left: 2px solid;
    border-bottom: 2px solid;
    position: absolute;
    left: 12px;
    top: 4px;
    transform: rotateZ(-45deg) rotateX(0deg);
    transition: .2s ease-in-out transform, .2s ease-in-out top;
    opacity: .75;
}

.dropdowncheck:checked ~ .dropdownlabel::before {
    transform: rotateZ(45deg) rotateX(180deg);
    top: 8px;
}

@media (prefers-color-scheme: dark) {
    body {
        background-color: #1a1a1a;
        color: var(--color-text-dark);
    }

    .nastaveni {
        background-color: #111;
    }

    .nastaveni .sett-card {
        background-color: #222;
        color: #eee;
        box-shadow: none;
    }

    .nastaveni .sett-header {
        box-shadow: 0 16px 16px #111;
    }

    .nastaveni::after {
        background-color: #111;
        box-shadow: 0 -8px 8px #111;
    }

    .nastaveni input, .nastaveni select, .nastaveni input:focus, .nastaveni button, label.dropdownlabel {
        color: #fff;
        background-color: rgba(255, 255, 255, .05);
        border: 1px solid rgba(255, 255, 255, .05);
        box-shadow: none;
    }

    .nastaveni .sett-card h4 {
        color: #fff;
    }

    .nastaveni .sett-card .sett-card-actions {
        border-color: #666;
    }

    .nastaveni hr, .nastaveni .sett-header button#cancel::before, .nastaveni .sett-header button#cancel::after, .nastaveni .sett-card h4::before {
        background-color: #999;
    }


    .hodiny {
        box-shadow: 8px 8px 32px rgba(0,0,0,.5), inset -8px -8px 32px rgba(255,255,255,.025), 0 0 0 calc(var(--cell-size) / 4) rgba(255,255,255,.05), 2px 2px 4px rgba(255,255,255,.05);
        background-image: linear-gradient(-45deg, #0d0d0d, #060606);
    }

    .nastaveni .sett-header button {
        box-shadow: none;
    }

    .nastaveni button.save{
        background-color: rgba(0, 255, 0, 0.25);
        box-shadow: none;
        border-color: rgba(0, 255, 0, .25);
        text-shadow: 0 0 16px #000;
        color: rgb(91, 255, 91);
    }
}

@media screen and (max-width: 100vh) {
    :root {
        --cell-size: calc(75vw / 11);
        --font-size: calc(var(--cell-size) * .6);
    }

    .nastaveni {
        min-width: calc(100% - 8px);
        transform: translateX(100%);
    }

    .sett .nastaveni {
        transform: translateX(0);
    }

    .sett .hodiny {
        right: 50vw;
    }
}


)=====";
