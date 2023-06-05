//  ****************************************************************************************************
//  Translator
//  ****************************************************************************************************
/*
import Translator from './translator.js'
const translator = new Translator({
  defaultLanguage: 'de',
  detectLanguage: true,
  selector: '[data-i18n]',
  debug: true,
  registerGlobally: '__',
  persist: true,
  persistKey: 'preferred_language',
  filesLocation: './i18n'
})

// By using `fetch`, you load the translation sources asynchronously
// from a directory in your project's folder. The resources must
// be in JSON. After they are fetched, you can use the API to
// translate the page.
translator.fetch(['de', 'en']).then(() => {
  // -> Translations are ready...
  translator.translatePageTo('de')
})
*/



//  ****************************************************************************************************
//  Color preview
//  ****************************************************************************************************

const closest = (to, selector) => {
	let currentElement = document.querySelector(to)
	let returnElement

	while (currentElement.parentNode && !returnElement) {
			currentElement = currentElement.parentNode
			returnElement = currentElement.querySelector(selector)
	}

	return returnElement
}



//  ****************************************************************************************************
//  FUNCTIONS - get
//  ****************************************************************************************************
//  Global
function getGloBrightness() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("gloBrightness").selectedIndex = "0";
			}
			if (this.responseText === "25") {
				document.getElementById("gloBrightness").selectedIndex = "1";
			}
			if (this.responseText === "50") {
				document.getElementById("gloBrightness").selectedIndex = "2";
			}
			if (this.responseText === "75") {
				document.getElementById("gloBrightness").selectedIndex = "3";
			}
			if (this.responseText === "100") {
				document.getElementById("gloBrightness").selectedIndex = "4";
			}
			if (this.responseText === "125") {
				document.getElementById("gloBrightness").selectedIndex = "5";
			}
			if (this.responseText === "150") {
				document.getElementById("gloBrightness").selectedIndex = "6";
			}
			if (this.responseText === "175") {
				document.getElementById("gloBrightness").selectedIndex = "7";
			}
			if (this.responseText === "200") {
				document.getElementById("gloBrightness").selectedIndex = "8";
			}
			if (this.responseText === "225") {
				document.getElementById("gloBrightness").selectedIndex = "9";
			}
			if (this.responseText === "250") {
				document.getElementById("gloBrightness").selectedIndex = "10";
			}
		}
	};
	xhttp.open("GET", "getGloBrightness", true);
	xhttp.send();
}

function getGloColorFreq() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("gloColorFreq").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("gloColorFreq").selectedIndex = "1";
			}
			if (this.responseText === "2") {
				document.getElementById("gloColorFreq").selectedIndex = "2";
			}
			if (this.responseText === "3") {
				document.getElementById("gloColorFreq").selectedIndex = "3";
			}
		}
	};
	xhttp.open("GET", "getGloColorFreq", true);
	xhttp.send();
}

function getDowUsage() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("dowUsage").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("dowUsage").selectedIndex = "1";
			}
		}
	};
	xhttp.open("GET", "getDowUsage", true);
	xhttp.send();
}

function getDowBrightness() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("dowBrightness").selectedIndex = "0";
			}
			if (this.responseText === "25") {
				document.getElementById("dowBrightness").selectedIndex = "1";
			}
			if (this.responseText === "50") {
				document.getElementById("dowBrightness").selectedIndex = "2";
			}
			if (this.responseText === "75") {
				document.getElementById("dowBrightness").selectedIndex = "3";
			}
			if (this.responseText === "100") {
				document.getElementById("dowBrightness").selectedIndex = "4";
			}
			if (this.responseText === "125") {
				document.getElementById("dowBrightness").selectedIndex = "5";
			}
			if (this.responseText === "150") {
				document.getElementById("dowBrightness").selectedIndex = "6";
			}
			if (this.responseText === "175") {
				document.getElementById("dowBrightness").selectedIndex = "7";
			}
			if (this.responseText === "200") {
				document.getElementById("dowBrightness").selectedIndex = "8";
			}
			if (this.responseText === "225") {
				document.getElementById("dowBrightness").selectedIndex = "9";
			}
			if (this.responseText === "250") {
				document.getElementById("dowBrightness").selectedIndex = "10";
			}
		}
	};
	xhttp.open("GET", "getDowBrightness", true);
	xhttp.send();
}

function getDowColorFreq() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("dowColorFreq").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("dowColorFreq").selectedIndex = "1";
			}
			if (this.responseText === "2") {
				document.getElementById("dowColorFreq").selectedIndex = "2";
			}
			if (this.responseText === "3") {
				document.getElementById("dowColorFreq").selectedIndex = "3";
			}
		}
	};
	xhttp.open("GET", "getDowColorFreq", true);
	xhttp.send();
}

//  KNX
function getKNXAddr() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxAddr").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXAddr", true);
	xhttp.send();
}

function getKNXPort() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxPort").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXPort", true);
	xhttp.send();
}

function getBriKNXPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("briKNXPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getBriKNXPath", true);
	xhttp.send();
}

function getTemKNXPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("temKNXPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getTemKNXPath", true);
	xhttp.send();
}

function getHumKNXPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humKNXPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getHumKNXPath", true);
	xhttp.send();
}

//  Clock
function getClkAddr() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkAddr").value = this.responseText;
		}
	};
	xhttp.open("GET", "getClkAddr", true);
	xhttp.send();
}

function getClkZone() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkZone").value = this.responseText;
		}
	};
	xhttp.open("GET", "getClkZone", true);
	xhttp.send();
}

function getClkFormat() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("clkFormat").selectedIndex = 0;
			}
			if (this.responseText === "1") {
				document.getElementById("clkFormat").selectedIndex = 1;
			}
		}
	};
	xhttp.open("GET", "getClkFormat", true);
	xhttp.send();
}

function getClkColorSet() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("clkColorSet").selectedIndex = 0;
				document.getElementById("clkFields").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("clkColorSet").selectedIndex = 1;
				document.getElementById("clkFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getClkColorSet", true);
	xhttp.send();
}

function getClkHColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkHColor").value = this.responseText;
			closest( '#clkHColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getClkHColor", true);
	xhttp.send();
}

function getClkMColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkMColor").value = this.responseText;
			closest( '#clkMColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getClkMColor", true);
	xhttp.send();
}

//  Date
function getDatFormat() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("datFormat").selectedIndex = 0;
			}
			if (this.responseText === "1") {
				document.getElementById("datFormat").selectedIndex = 1;
			}
		}
	};
	xhttp.open("GET", "getDatFormat", true);
	xhttp.send();
}

function getDatColorSet() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("datColorSet").selectedIndex = 0;
				document.getElementById("datFields").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("datColorSet").selectedIndex = 1;
				document.getElementById("datFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getDatColorSet", true);
	xhttp.send();
}

function getDatDColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("datDColor").value = this.responseText;
			closest( '#datDColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDatDColor", true);
	xhttp.send();
}

function getDatMColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("datMColor").value = this.responseText;
			closest( '#datMColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDatMColor", true);
	xhttp.send();
}

//  Temperature
function getTemSymbol() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("temSymbol").selectedIndex = 0;
			}
			if (this.responseText === "1") {
				document.getElementById("temSymbol").selectedIndex = 1;
			}
		}
	};
	xhttp.open("GET", "getTemSymbol", true);
	xhttp.send();
}

function getTemColorSet() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("temColorSet").selectedIndex = 0;
				document.getElementById("temFields").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("temColorSet").selectedIndex = 1;
				document.getElementById("temFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getTemColorSet", true);
	xhttp.send();
}

function getTemAColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("temAColor").value = this.responseText;
			closest( '#temAColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getTemAColor", true);
	xhttp.send();
}

function getTemBColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("temBColor").value = this.responseText;
			closest( '#temBColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getTemBColor", true);
	xhttp.send();
}

function getTemCColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("temCColor").value = this.responseText;
			closest( '#temCColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getTemCColor", true);
	xhttp.send();
}

//  Humidity
function getHumColorSet() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("humColorSet").selectedIndex = 0;
				document.getElementById("humFields").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("humColorSet").selectedIndex = 1;
				document.getElementById("humFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getHumColorSet", true);
	xhttp.send();
}

function getHumAColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humAColor").value = this.responseText;
			closest( '#humAColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumAColor", true);
	xhttp.send();
}

function getHumBColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humBColor").value = this.responseText;
			closest( '#humBColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumBColor", true);
	xhttp.send();
}

function getHumCColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humCColor").value = this.responseText;
			closest( '#humCColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumCColor", true);
	xhttp.send();
}

//  Scoreboard
function getScoHColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("scoHColor").value = this.responseText;
			closest( '#scoHColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getScoHColor", true);
	xhttp.send();
}

function getScoAColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("scoAColor").value = this.responseText;
			closest( '#scoAColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getScoAColor", true);
	xhttp.send();
}

//  Countdown
function getCouHColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couHColor").value = this.responseText;
			closest( '#couHColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouHColor", true);
	xhttp.send();
}

function getCouMColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couMColor").value = this.responseText;
			closest( '#couMColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouMColor", true);
	xhttp.send();
}

function getCouSColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couSColor").value = this.responseText;
			closest( '#couSColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouSColor", true);
	xhttp.send();
}

function getCouAColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couAColor").value = this.responseText;
			closest( '#couAColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouAColor", true);
	xhttp.send();
}

//  Downlights
function getDowColorSet() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("dowColorSet").selectedIndex = 0;
				document.getElementById("dowFields0").hidden = false;
				document.getElementById("dowFields1upper").hidden = true;
				document.getElementById("dowFields1lower").hidden = true;
			}
			if (this.responseText === "1") {
				document.getElementById("dowColorSet").selectedIndex = 1;
				document.getElementById("dowFields0").hidden = true;
				document.getElementById("dowFields1upper").hidden = false;
				document.getElementById("dowFields1lower").hidden = false;
			}
			if (this.responseText === "2") {
				document.getElementById("dowColorSet").selectedIndex = 2;
				document.getElementById("dowFields0").hidden = true;
				document.getElementById("dowFields1upper").hidden = true;
				document.getElementById("dowFields1lower").hidden = true;
			}
			if (this.responseText === "3") {
				document.getElementById("dowColorSet").selectedIndex = 3;
				document.getElementById("dowFields0").hidden = true;
				document.getElementById("dowFields1upper").hidden = true;
				document.getElementById("dowFields1lower").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getDowColorSet", true);
	xhttp.send();
}

function getDowColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor").value = this.responseText;
			closest( '#dowColor', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor", true);
	xhttp.send();
}

function getDowColor0() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor0").value = this.responseText;
			closest( '#dowColor0', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor0", true);
	xhttp.send();
}

function getDowColor1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor1").value = this.responseText;
			closest( '#dowColor1', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor1", true);
	xhttp.send();
}

function getDowColor2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor2").value = this.responseText;
			closest( '#dowColor2', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor2", true);
	xhttp.send();
}

function getDowColor3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor3").value = this.responseText;
			closest( '#dowColor3', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor3", true);
	xhttp.send();
}

function getDowColor4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor4").value = this.responseText;
			closest( '#dowColor4', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor4", true);
	xhttp.send();
}

function getDowColor5() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor5").value = this.responseText;
			closest( '#dowColor5', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor5", true);
	xhttp.send();
}

function getDowColor6() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor6").value = this.responseText;
			closest( '#dowColor6', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor6", true);
	xhttp.send();
}

function getDowColor7() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor7").value = this.responseText;
			closest( '#dowColor7', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor7", true);
	xhttp.send();
}

function getDowColor8() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor8").value = this.responseText;
			closest( '#dowColor8', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor8", true);
	xhttp.send();
}

function getDowColor9() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor9").value = this.responseText;
			closest( '#dowColor9', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor9", true);
	xhttp.send();
}

function getDowColor10() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor10").value = this.responseText;
			closest( '#dowColor10', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor10", true);
	xhttp.send();
}

function getDowColor11() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor11").value = this.responseText;
			closest( '#dowColor11', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor11", true);
	xhttp.send();
}

function getDowColor12() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor12").value = this.responseText;
			closest( '#dowColor12', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor12", true);
	xhttp.send();
}

function getDowColor13() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("dowColor13").value = this.responseText;
			closest( '#dowColor13', '.input-group-text' ).style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor13", true);
	xhttp.send();
}

//  ****************************************************************************************************
//  EVENT LISTENER - get
//  ****************************************************************************************************
function init() {
	//  Global
	getGloBrightness();
	getGloColorFreq();
	getDowUsage();
	getDowBrightness();
	getDowColorFreq();
	//  KNX
	getKNXAddr();
	getKNXPort();
	getBriKNXPath();
	getTemKNXPath();
	getHumKNXPath();
	//  Clock
	getClkAddr();
	getClkZone();
	getClkFormat();
	getClkColorSet();
	getClkHColor();
	getClkMColor();
	//  Date
	getDatFormat();
	getDatColorSet();
	getDatDColor();
	getDatMColor();
	//  Temperature
	getTemSymbol();
	getTemColorSet();
	getTemAColor();
	getTemBColor();
	getTemCColor();
	//  Humidity
	getHumColorSet();
	getHumAColor();
	getHumBColor();
	getHumCColor();
	//  Scoreboard
	getScoHColor();
	getScoAColor();
	//  Countdown
	getCouHColor();
	getCouMColor();
	getCouSColor();
	getCouAColor();
	//  Downlights
	getDowColorSet();
	getDowColor();
	getDowColor0();
	getDowColor1();
	getDowColor2();
	getDowColor3();
	getDowColor4();
	getDowColor5();
	getDowColor6();
	getDowColor7();
	getDowColor8();
	getDowColor9();
	getDowColor10();
	getDowColor11();
	getDowColor12();
	getDowColor13();
}

//  ****************************************************************************************************
//  EVENT LISTENER - update
//  ****************************************************************************************************
function startup() {
	// GLOBAL
	const gloBrightness = document.querySelector("#gloBrightness");
	gloBrightness.addEventListener("input", updGloBrightness, false);
	const gloColorFreq = document.querySelector("#gloColorFreq");
	gloColorFreq.addEventListener("input", updGloColorFreq, false);
	const dowUsage = document.querySelector("#dowUsage");
	dowUsage.addEventListener("input", updDowUsage, false);
	const dowBrightness = document.querySelector("#dowBrightness");
	dowBrightness.addEventListener("input", updDowBrightness, false);
	const dowColorFreq = document.querySelector("#dowColorFreq");
	dowColorFreq.addEventListener("input", updDowColorFreq, false);
	// KNX
	const knxAddr = document.querySelector("#knxAddr");
	knxAddr.addEventListener("input", updKNXAddr, false);
	const knxPort = document.querySelector("#knxPort");
	knxPort.addEventListener("input", updKNXPort, false);
	const briKNXPath = document.querySelector("#briKNXPath");
	briKNXPath.addEventListener("input", updBriKNXPath, false);
	const temKNXPath = document.querySelector("#temKNXPath");
	temKNXPath.addEventListener("input", updTemKNXPath, false);
	const humKNXPath = document.querySelector("#humKNXPath");
	humKNXPath.addEventListener("input", updHumKNXPath, false);
	//  CLOCK
	const clkAddr = document.querySelector("#clkAddr");
	clkAddr.addEventListener("input", updClkAddr, false);
	const clkZone = document.querySelector("#clkZone");
	clkZone.addEventListener("input", updClkZone, false);
	const clkFormat = document.querySelector("#clkFormat");
	clkFormat.addEventListener("input", updClkFormat, false);
	const clkColorSet = document.querySelector("#clkColorSet");
	clkColorSet.addEventListener("input", updClkColorSet, false);
	const clkHColor = document.querySelector("#clkHColor");
	clkHColor.addEventListener("input", updClkHColor, false);
	clkHColor.addEventListener("change", updClkHColor, false);
	const clkMColor = document.querySelector("#clkMColor");
	clkMColor.addEventListener("input", updClkMColor, false);
	clkMColor.addEventListener("change", updClkMColor, false);
	//  Date
	const datFormat = document.querySelector("#datFormat");
	datFormat.addEventListener("input", updDatFormat, false);
	const datColorSet = document.querySelector("#datColorSet");
	datColorSet.addEventListener("input", updDatColorSet, false);
	const datDColor = document.querySelector("#datDColor");
	datDColor.addEventListener("input", updDatDColor, false);
	datDColor.addEventListener("change", updDatDColor, false);
	const datMColor = document.querySelector("#datMColor");
	datMColor.addEventListener("input", updDatMColor, false);
	datMColor.addEventListener("change", updDatMColor, false);
	//  Temperature
	const temSymbol = document.querySelector("#temSymbol");
	temSymbol.addEventListener("input", updTemSymbol, false);
	const temColorSet = document.querySelector("#temColorSet");
	temColorSet.addEventListener("input", updTemColorSet, false);
	const temAColor = document.querySelector("#temAColor");
	temAColor.addEventListener("input", updTemAColor, false);
	temAColor.addEventListener("change", updTemAColor, false);
	const temBColor = document.querySelector("#temBColor");
	temBColor.addEventListener("input", updTemBColor, false);
	temBColor.addEventListener("change", updTemBColor, false);
	const temCColor = document.querySelector("#temCColor");
	temCColor.addEventListener("input", updTemCColor, false);
	temCColor.addEventListener("change", updTemCColor, false);
	//  Humidity
	const humColorSet = document.querySelector("#humColorSet");
	humColorSet.addEventListener("input", updHumColorSet, false);
	const humAColor = document.querySelector("#humAColor");
	humAColor.addEventListener("input", updHumAColor, false);
	humAColor.addEventListener("change", updHumAColor, false);
	const humBColor = document.querySelector("#humBColor");
	humBColor.addEventListener("input", updHumBColor, false);
	humBColor.addEventListener("change", updHumBColor, false);
	const humCColor = document.querySelector("#humCColor");
	humCColor.addEventListener("input", updHumCColor, false);
	humCColor.addEventListener("change", updHumCColor, false);
	//  Scoreboard
	const scoHColor = document.querySelector("#scoHColor");
	scoHColor.addEventListener("input", updScoHColor, false);
	scoHColor.addEventListener("change", updScoHColor, false);
	const scoAColor = document.querySelector("#scoAColor");
	scoAColor.addEventListener("input", updScoAColor, false);
	scoAColor.addEventListener("change", updScoAColor, false);
	//  Countdown
	const couHColor = document.querySelector("#couHColor");
	couHColor.addEventListener("input", updCouHColor, false);
	couHColor.addEventListener("change", updCouHColor, false);
	const couMColor = document.querySelector("#couMColor");
	couMColor.addEventListener("input", updCouMColor, false);
	couMColor.addEventListener("change", updCouMColor, false);
	const couSColor = document.querySelector("#couSColor");
	couSColor.addEventListener("input", updCouSColor, false);
	couSColor.addEventListener("change", updCouSColor, false);
	const couAColor = document.querySelector("#couAColor");
	couAColor.addEventListener("input", updCouAColor, false);
	couAColor.addEventListener("change", updCouAColor, false);
	//  Downlights
	const dowColorSet = document.querySelector("#dowColorSet");
	dowColorSet.addEventListener("input", updDowColorSet, false);
	const dowColor = document.querySelector("#dowColor");
	dowColor.addEventListener("input", updDowColor, false);
	dowColor.addEventListener("change", updDowColor, false);
	const dowColor0 = document.querySelector("#dowColor0");
	dowColor0.addEventListener("input", updDowColor0, false);
	dowColor0.addEventListener("change", updDowColor0, false);
	const dowColor1 = document.querySelector("#dowColor1");
	dowColor1.addEventListener("input", updDowColor1, false);
	dowColor1.addEventListener("change", updDowColor1, false);
	const dowColor2 = document.querySelector("#dowColor2");
	dowColor2.addEventListener("input", updDowColor2, false);
	dowColor2.addEventListener("change", updDowColor2, false);
	const dowColor3 = document.querySelector("#dowColor3");
	dowColor3.addEventListener("input", updDowColor3, false);
	dowColor3.addEventListener("change", updDowColor3, false);
	const dowColor4 = document.querySelector("#dowColor4");
	dowColor4.addEventListener("input", updDowColor4, false);
	dowColor4.addEventListener("change", updDowColor4, false);
	const dowColor5 = document.querySelector("#dowColor5");
	dowColor5.addEventListener("input", updDowColor5, false);
	dowColor5.addEventListener("change", updDowColor5, false);
	const dowColor6 = document.querySelector("#dowColor6");
	dowColor6.addEventListener("input", updDowColor6, false);
	dowColor6.addEventListener("change", updDowColor6, false);
	const dowColor7 = document.querySelector("#dowColor7");
	dowColor7.addEventListener("input", updDowColor7, false);
	dowColor7.addEventListener("change", updDowColor7, false);
	const dowColor8 = document.querySelector("#dowColor8");
	dowColor8.addEventListener("input", updDowColor8, false);
	dowColor8.addEventListener("change", updDowColor8, false);
	const dowColor9 = document.querySelector("#dowColor9");
	dowColor9.addEventListener("input", updDowColor9, false);
	dowColor9.addEventListener("change", updDowColor9, false);
	const dowColor10 = document.querySelector("#dowColor10");
	dowColor10.addEventListener("input", updDowColor10, false);
	dowColor10.addEventListener("change", updDowColor10, false);
	const dowColor11 = document.querySelector("#dowColor11");
	dowColor11.addEventListener("input", updDowColor11, false);
	dowColor11.addEventListener("change", updDowColor11, false);
	const dowColor12 = document.querySelector("#dowColor12");
	dowColor12.addEventListener("input", updDowColor12, false);
	dowColor12.addEventListener("change", updDowColor12, false);
	const dowColor13 = document.querySelector("#dowColor13");
	dowColor13.addEventListener("input", updDowColor13, false);
	dowColor13.addEventListener("change", updDowColor13, false);
}

//  ****************************************************************************************************
//  FUNCTIONS - upd
//  ****************************************************************************************************
function hexToRgb(hex) {
	const shorthandRegex = /^#?([a-f\d])([a-f\d])([a-f\d])$/i;
	hex = hex.replace(shorthandRegex, function (m, r, g, b) {
		return r + r + g + g + b + b;
	});
	const result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
	return result
		? {
				r: parseInt(result[1], 16),
				g: parseInt(result[2], 16),
				b: parseInt(result[3], 16),
		  }
		: null;
}

//  Global
function updGloBrightness(event) {
	const value = event.target.value;
	$.post("/updGloBrightness", { gloBrightness: value });
	console.log(value);
}

function updGloColorFreq(event) {
	const value = event.target.value;
	$.post("/updGloColorFreq", { gloColorFreq: value });
	console.log(value);
}

function updDowUsage(event) {
	const value = event.target.value;
	$.post("/updDowUsage", { dowUsage: value });
	console.log(value);
}

function updDowBrightness(event) {
	const value = event.target.value;
	$.post("/updDowBrightness", { dowBrightness: value });
	console.log(value);
}

function updDowColorFreq(event) {
	const value = event.target.value;
	$.post("/updDowColorFreq", { dowColorFreq: value });
	console.log(value);
}

//  KNX
function updKNXAddr(event) {
	const value = event.target.value;
	$.post("/updKNXAddr", { knxAddr: value });
	console.log(value);
}

function updKNXPort(event) {
	const value = event.target.value;
	$.post("/updKNXPort", { knxPort: value });
	console.log(value);
}

function updBriKNXPath(event) {
	const value = event.target.value;
	$.post("/updBriKNXPath", { briKNXPath: value });
	console.log(value);
}

function updTemKNXPath(event) {
	const value = event.target.value;
	$.post("/updTemKNXPath", { temKNXPath: value });
	console.log(value);
}

function updHumKNXPath(event) {
	const value = event.target.value;
	$.post("/updHumKNXPath", { humKNXPath: value });
	console.log(value);
}

//  Clock
function updClkAddr(event) {
	const value = event.target.value;
	$.post("/updClkAddr", { clkAddr: value });
	console.log(value);
}

function updClkZone(event) {
	const value = event.target.value;
	$.post("/updClkZone", { clkZone: value });
	console.log(value);
}

function updClkFormat(event) {
	const value = event.target.value;
	$.post("/updClkFormat", { clkFormat: value });
	console.log(value);
}

function updClkColorSet(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("clkFields").hidden = false;
	}
	if (value === "1") {
		document.getElementById("clkFields").hidden = true;
	}
	$.post("/updClkColorSet", { clkColorSet: value });
	console.log(value);
}

function updClkHColor(event) {
	const value = event.target.value;
	$.post("/updClkHColor", hexToRgb(value));
	closest( '#clkHColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updClkMColor(event) {
	const value = event.target.value;
	$.post("/updClkMColor", hexToRgb(value));
	closest( '#clkMColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Date
function updDatFormat(event) {
	const value = event.target.value;
	$.post("/updDatFormat", { datFormat: value });
	console.log(value);
}

function updDatColorSet(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("datFields").hidden = false;
	}
	if (value === "1") {
		document.getElementById("datFields").hidden = true;
	}

	$.post("/updDatColorSet", { datColorSet: value });
	console.log(value);
}

function updDatDColor(event) {
	const value = event.target.value;
	$.post("/updDatDColor", hexToRgb(value));
	closest( '#datDColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDatMColor(event) {
	const value = event.target.value;
	$.post("/updDatMColor", hexToRgb(value));
	closest( '#datMColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Temperature
function updTemSymbol(event) {
	const value = event.target.value;
	$.post("/updTemSymbol", { temSymbol: value });
	console.log(value);
}

function updTemColorSet(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("temFields").hidden = false;
	}
	if (value === "1") {
		document.getElementById("temFields").hidden = true;
	}
	$.post("/updTemColorSet", { temColorSet: value });
	console.log(value);
}

function updTemAColor(event) {
	const value = event.target.value;
	$.post("/updTemAColor", hexToRgb(value));
	closest( '#temAColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updTemBColor(event) {
	const value = event.target.value;
	$.post("/updTemBColor", hexToRgb(value));
	closest( '#temBColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updTemCColor(event) {
	const value = event.target.value;
	$.post("/updTemCColor", hexToRgb(value));
	closest( '#temCColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Humidity
function updHumColorSet(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("humFields").hidden = false;
	}
	if (value === "1") {
		document.getElementById("humFields").hidden = true;
	}
	$.post("/updHumColorSet", { humColorSet: value });
	console.log(value);
}

function updHumAColor(event) {
	const value = event.target.value;
	$.post("/updHumAColor", hexToRgb(value));
	closest( '#humAColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updHumBColor(event) {
	const value = event.target.value;
	$.post("/updHumBColor", hexToRgb(value));
	closest( '#humBColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updHumCColor(event) {
	const value = event.target.value;
	$.post("/updHumCColor", hexToRgb(value));
	closest( '#humCColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Scoreboard
function updScoHColor(event) {
	const value = event.target.value;
	$.post("/updScoHColor", hexToRgb(value));
	closest( '#scoHColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updScoAColor(event) {
	const value = event.target.value;
	$.post("/updScoAColor", hexToRgb(value));
	closest( '#scoAColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Countdown
function updCouHColor(event) {
	const value = event.target.value;
	$.post("/updCouHColor", hexToRgb(value));
	closest( '#couHColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updCouMColor(event) {
	const value = event.target.value;
	$.post("/updCouMColor", hexToRgb(value));
	closest( '#couMColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updCouSColor(event) {
	const value = event.target.value;
	$.post("/updCouSColor", hexToRgb(value));
	closest( '#couSColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updCouAColor(event) {
	const value = event.target.value;
	$.post("/updCouAColor", hexToRgb(value));
	closest( '#couAColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Downlights
function updDowColorSet(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("dowFields0").hidden = false;
		document.getElementById("dowFields1upper").hidden = true;
		document.getElementById("dowFields1lower").hidden = true;
	}
	if (value === "1") {
		document.getElementById("dowFields0").hidden = true;
		document.getElementById("dowFields1upper").hidden = false;
		document.getElementById("dowFields1lower").hidden = false;
	}
	if (value === "2") {
		document.getElementById("dowFields0").hidden = true;
		document.getElementById("dowFields1upper").hidden = true;
		document.getElementById("dowFields1lower").hidden = true;
	}
	if (value === "3") {
		document.getElementById("dowFields0").hidden = true;
		document.getElementById("dowFields1upper").hidden = true;
		document.getElementById("dowFields1lower").hidden = true;
	}
	$.post("/updDowColorSet", { dowColorSet: value });
	console.log(value);
}

function updDowColor(event) {
	const value = event.target.value;
	$.post("/updDowColor", hexToRgb(value));
	closest( '#dowColor', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor0(event) {
	const value = event.target.value;
	$.post("/updDowColor0", hexToRgb(value));
	closest( '#dowColor0', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor1(event) {
	const value = event.target.value;
	$.post("/updDowColor1", hexToRgb(value));
	closest( '#dowColor1', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor2(event) {
	const value = event.target.value;
	$.post("/updDowColor2", hexToRgb(value));
	closest( '#dowColor2', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor3(event) {
	const value = event.target.value;
	$.post("/updDowColor3", hexToRgb(value));
	closest( '#dowColor3', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor4(event) {
	const value = event.target.value;
	$.post("/updDowColor4", hexToRgb(value));
	closest( '#dowColor4', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor5(event) {
	const value = event.target.value;
	$.post("/updDowColor5", hexToRgb(value));
	closest( '#dowColor5', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor6(event) {
	const value = event.target.value;
	$.post("/updDowColor6", hexToRgb(value));
	closest( '#dowColor6', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor7(event) {
	const value = event.target.value;
	$.post("/updDowColor7", hexToRgb(value));
	closest( '#dowColor7', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor8(event) {
	const value = event.target.value;
	$.post("/updDowColor8", hexToRgb(value));
	closest( '#dowColor8', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor9(event) {
	const value = event.target.value;
	$.post("/updDowColor9", hexToRgb(value));
	closest( '#dowColor9', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor10(event) {
	const value = event.target.value;
	$.post("/updDowColor10", hexToRgb(value));
	closest( '#dowColor10', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor11(event) {
	const value = event.target.value;
	$.post("/updDowColor11", hexToRgb(value));
	closest( '#dowColor11', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor12(event) {
	const value = event.target.value;
	$.post("/updDowColor12", hexToRgb(value));
	closest( '#dowColor12', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}

function updDowColor13(event) {
	const value = event.target.value;
	$.post("/updDowColor13", hexToRgb(value));
	closest( '#dowColor13', '.input-group-text' ).style.backgroundColor = value;
	console.log(hexToRgb(value));
}
