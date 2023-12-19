//  ****************************************************************************************************
//  Color preview
//  ****************************************************************************************************
const closest = (to, selector) => {
	let currentElement = document.querySelector(to);
	let returnElement;

	while (currentElement.parentNode && !returnElement) {
		currentElement = currentElement.parentNode;
		returnElement = currentElement.querySelector(selector);
	}

	return returnElement;
};

//  ****************************************************************************************************
//  FUNCTIONS - get
//  ****************************************************************************************************
//  Globale Einstellungen
function getEdgeBrightness() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("edgeBrightness").selectedIndex = "0";
			}
			if (this.responseText === "25") {
				document.getElementById("edgeBrightness").selectedIndex = "1";
			}
			if (this.responseText === "50") {
				document.getElementById("edgeBrightness").selectedIndex = "2";
			}
			if (this.responseText === "75") {
				document.getElementById("edgeBrightness").selectedIndex = "3";
			}
			if (this.responseText === "100") {
				document.getElementById("edgeBrightness").selectedIndex = "4";
			}
			if (this.responseText === "125") {
				document.getElementById("edgeBrightness").selectedIndex = "5";
			}
			if (this.responseText === "150") {
				document.getElementById("edgeBrightness").selectedIndex = "6";
			}
			if (this.responseText === "175") {
				document.getElementById("edgeBrightness").selectedIndex = "7";
			}
			if (this.responseText === "200") {
				document.getElementById("edgeBrightness").selectedIndex = "8";
			}
			if (this.responseText === "225") {
				document.getElementById("edgeBrightness").selectedIndex = "9";
			}
			if (this.responseText === "250") {
				document.getElementById("edgeBrightness").selectedIndex = "10";
			}
		}
	};
	xhttp.open("GET", "getEdgeBrightness", true);
	xhttp.send();
}
function getEdgeBrightnessMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("edgeBrightnessMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getEdgeBrightnessMin", true);
	xhttp.send();
}
function getEdgeBrightnessMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("edgeBrightnessMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getEdgeBrightnessMax", true);
	xhttp.send();
}
function getEdgeFrequency() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("edgeFrequency").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("edgeFrequency").selectedIndex = "1";
			}
			if (this.responseText === "2") {
				document.getElementById("edgeFrequency").selectedIndex = "2";
			}
			if (this.responseText === "3") {
				document.getElementById("edgeFrequency").selectedIndex = "3";
			}
		}
	};
	xhttp.open("GET", "getEdgeFrequency", true);
	xhttp.send();
}
function getDownlightUsage() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downlightUsage").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("downlightUsage").selectedIndex = "1";
			}
		}
	};
	xhttp.open("GET", "getDownlightUsage", true);
	xhttp.send();
}
function getDownlightBrightness() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downlightBrightness").selectedIndex = "0";
			}
			if (this.responseText === "25") {
				document.getElementById("downlightBrightness").selectedIndex = "1";
			}
			if (this.responseText === "50") {
				document.getElementById("downlightBrightness").selectedIndex = "2";
			}
			if (this.responseText === "75") {
				document.getElementById("downlightBrightness").selectedIndex = "3";
			}
			if (this.responseText === "100") {
				document.getElementById("downlightBrightness").selectedIndex = "4";
			}
			if (this.responseText === "125") {
				document.getElementById("downlightBrightness").selectedIndex = "5";
			}
			if (this.responseText === "150") {
				document.getElementById("downlightBrightness").selectedIndex = "6";
			}
			if (this.responseText === "175") {
				document.getElementById("downlightBrightness").selectedIndex = "7";
			}
			if (this.responseText === "200") {
				document.getElementById("downlightBrightness").selectedIndex = "8";
			}
			if (this.responseText === "225") {
				document.getElementById("downlightBrightness").selectedIndex = "9";
			}
			if (this.responseText === "250") {
				document.getElementById("downlightBrightness").selectedIndex = "10";
			}
		}
	};
	xhttp.open("GET", "getDownlightBrightness", true);
	xhttp.send();
}
function getDownlightBrightnessMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downlightBrightnessMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getDownlightBrightnessMin", true);
	xhttp.send();
}
function getDownlightBrightnessMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downlightBrightnessMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getDownlightBrightnessMax", true);
	xhttp.send();
}
function getDownlightFrequency() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downlightFrequency").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("downlightFrequency").selectedIndex = "1";
			}
			if (this.responseText === "2") {
				document.getElementById("downlightFrequency").selectedIndex = "2";
			}
			if (this.responseText === "3") {
				document.getElementById("downlightFrequency").selectedIndex = "3";
			}
		}
	};
	xhttp.open("GET", "getDownlightFrequency", true);
	xhttp.send();
}

//  KNX
function getKNXAddress() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxAddress").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXAddress", true);
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
function getKNXBrightnessPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightnessPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightnessPath", true);
	xhttp.send();
}
function getKNXBrightnessMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightnessMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightnessMin", true);
	xhttp.send();
}
function getKNXBrightnessMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightnessMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightnessMax", true);
	xhttp.send();
}
function getKNXTemperaturePath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxTemperaturePath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXTemperaturePath", true);
	xhttp.send();
}
function getKNXHumidityPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxHumidityPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXHumidityPath", true);
	xhttp.send();
}

//  Clock
function getClkAddress() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkAddress").value = this.responseText;
		}
	};
	xhttp.open("GET", "getClkAddress", true);
	xhttp.send();
}
function getClkTimeZone() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkTimeZone").value = this.responseText;
		}
	};
	xhttp.open("GET", "getClkTimeZone", true);
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
function getClkColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("clkColor").selectedIndex = 0;
				document.getElementById("clkColorInput").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("clkColor").selectedIndex = 1;
				document.getElementById("clkColorInput").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getClkColor", true);
	xhttp.send();
}
function getClkColorH() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkColorH").value = this.responseText;
			closest("#clkColorH", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getClkColorH", true);
	xhttp.send();
}
function getClkColorM() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("clkColorM").value = this.responseText;
			closest("#clkColorM", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getClkColorM", true);
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
function getDatColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("datColor").selectedIndex = 0;
				document.getElementById("datColorInput").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("datColor").selectedIndex = 1;
				document.getElementById("datColorInput").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getDatColor", true);
	xhttp.send();
}
function getDatColorD() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("datColorD").value = this.responseText;
			closest("#datColorD", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDatColorD", true);
	xhttp.send();
}
function getDatColorM() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("datColorM").value = this.responseText;
			closest("#datColorM", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDatColorM", true);
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

//  Humidity
function getHumColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("humColor").selectedIndex = 0;
				document.getElementById("humColorInput").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("humColor").selectedIndex = 1;
				document.getElementById("humColorInput").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getHumColor", true);
	xhttp.send();
}
function getHumColorA() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humColorA").value = this.responseText;
			closest("#humColorA", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumColorA", true);
	xhttp.send();
}
function getHumColorB() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humColorB").value = this.responseText;
			closest("#humColorB", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumColorB", true);
	xhttp.send();
}
function getHumColorC() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("humColorC").value = this.responseText;
			closest("#humColorC", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getHumColorC", true);
	xhttp.send();
}

//  Scoreboard
function getScoColorL() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("scoColorL").value = this.responseText;
			closest("#scoColorL", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getScoColorL", true);
	xhttp.send();
}
function getScoColorR() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("scoColorR").value = this.responseText;
			closest("#scoColorR", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getScoColorR", true);
	xhttp.send();
}

//  Countdown
function getCouColorH() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couColorH").value = this.responseText;
			closest("#couColorH", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouColorH", true);
	xhttp.send();
}
function getCouColorM() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couColorM").value = this.responseText;
			closest("#couColorM", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouColorM", true);
	xhttp.send();
}
function getCouColorS() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couColorS").value = this.responseText;
			closest("#couColorS", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouColorS", true);
	xhttp.send();
}
function getCouColorA() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("couColorA").value = this.responseText;
			closest("#couColorA", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getCouColorA", true);
	xhttp.send();
}

//	Scroll
function getScrTime() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("scrTime").value = this.responseText;
		}
	};
	xhttp.open("GET", "getScrTime", true);
	xhttp.send();
}

//  Downlights
function getDowColor() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("dowColor").selectedIndex = 0;
				document.getElementById("dowColorInput").hidden = false;
			}
			if (this.responseText === "1") {
				document.getElementById("dowColor").selectedIndex = 1;
				document.getElementById("dowColorInput").hidden = true;
			}
			if (this.responseText === "2") {
				document.getElementById("dowColor").selectedIndex = 2;
				document.getElementById("dowColorInput").hidden = true;
			}
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
			closest("#dowColor0", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor1", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor2", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor3", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor4", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor5", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor6", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor7", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor8", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor9", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor10", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor11", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor12", ".input-group-text").style.backgroundColor = this.responseText;
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
			closest("#dowColor13", ".input-group-text").style.backgroundColor = this.responseText;
		}
	};
	xhttp.open("GET", "getDowColor13", true);
	xhttp.send();
}

//  ****************************************************************************************************
//  EVENT LISTENER - get
//  ****************************************************************************************************
function init() {
	//  Globale Einstellungen
	//  |- Edge
	getEdgeBrightness();
	getEdgeBrightnessMin();
	getEdgeBrightnessMax();
	getEdgeFrequency();
	//  |- Downlights
	getDownlightUsage();
	getDownlightBrightness();
	getDownlightBrightnessMin();
	getDownlightBrightnessMax();
	getDownlightFrequency();

	//  KNX
	getKNXAddress();
	getKNXPort();
	getKNXBrightnessPath();
	getKNXBrightnessMin();
	getKNXBrightnessMax();
	getKNXTemperaturePath();
	getKNXHumidityPath();

	//  Clock
	getClkAddress();
	getClkTimeZone();
	getClkFormat();
	getClkColor();
	getClkColorH();
	getClkColorM();

	//  Date
	getDatFormat();
	getDatColor();
	getDatColorD();
	getDatColorM();

	//  Temperature
	getTemSymbol();

	//  Humidity
	getHumColor();
	getHumColorA();
	getHumColorB();
	getHumColorC();

	//  Scoreboard
	getScoColorL();
	getScoColorR();

	//  Countdown
	getCouColorH();
	getCouColorM();
	getCouColorS();
	getCouColorA();

	//	Scroll
	getScrTime();

	//  Downlights
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
	//  Globale Einstellungen
	//  |- Edge
	const edgeBrightness = document.querySelector("#edgeBrightness");
	edgeBrightness.addEventListener("input", updateEdgeBrightness, false);
	const edgeBrightnessMin = document.querySelector("#edgeBrightnessMin");
	edgeBrightnessMin.addEventListener("input", updateEdgeBrightnessMin, false);
	const edgeBrightnessMax = document.querySelector("#edgeBrightnessMax");
	edgeBrightnessMax.addEventListener("input", updateEdgeBrightnessMax, false);
	const edgeFrequency = document.querySelector("#edgeFrequency");
	edgeFrequency.addEventListener("input", updateEdgeFrequency, false);
	//  |- Downlights
	const downlightUsage = document.querySelector("#downlightUsage");
	downlightUsage.addEventListener("input", updateDownlightUsage, false);
	const downlightBrightness = document.querySelector("#downlightBrightness");
	downlightBrightness.addEventListener("input", updateDownlightBrightness, false);
	const downlightBrightnessMin = document.querySelector("#downlightBrightnessMin");
	downlightBrightnessMin.addEventListener("input", updateDownlightBrightnessMin, false);
	const downlightBrightnessMax = document.querySelector("#downlightBrightnessMax");
	downlightBrightnessMax.addEventListener("input", updateDownlightBrightnessMax, false);
	const downlightFrequency = document.querySelector("#downlightFrequency");
	downlightFrequency.addEventListener("input", updateDownlightFrequency, false);

	// KNX
	const knxAddress = document.querySelector("#knxAddress");
	knxAddress.addEventListener("input", updateKNXAddress, false);
	const knxPort = document.querySelector("#knxPort");
	knxPort.addEventListener("input", updateKNXPort, false);
	const knxBrightnessPath = document.querySelector("#knxBrightnessPath");
	knxBrightnessPath.addEventListener("input", updateKNXBrightnessPath, false);
	const knxBrightnessMin = document.querySelector("#knxBrightnessMin");
	knxBrightnessMin.addEventListener("input", updateKNXBrightnessMin, false);
	const knxBrightnessMax = document.querySelector("#knxBrightnessMax");
	knxBrightnessMax.addEventListener("input", updateKNXBrightnessMax, false);
	const knxTemperaturePath = document.querySelector("#knxTemperaturePath");
	knxTemperaturePath.addEventListener("input", updateKNXTemperaturePath, false);
	const knxHumidityPath = document.querySelector("#knxHumidityPath");
	knxHumidityPath.addEventListener("input", updateKNXHumidityPath, false);

	//  Clock
	const clkAddress = document.querySelector("#clkAddress");
	clkAddress.addEventListener("input", updateClkAddress, false);
	const clkTimeZone = document.querySelector("#clkTimeZone");
	clkTimeZone.addEventListener("input", updateClkTimeZone, false);
	const clkFormat = document.querySelector("#clkFormat");
	clkFormat.addEventListener("input", updateClkFormat, false);
	const clkColor = document.querySelector("#clkColor");
	clkColor.addEventListener("input", updateClkColor, false);
	const clkColorH = document.querySelector("#clkColorH");
	clkColorH.addEventListener("input", updateClkColorH, false);
	clkColorH.addEventListener("change", updateClkColorH, false);
	const clkColorM = document.querySelector("#clkColorM");
	clkColorM.addEventListener("input", updateClkColorM, false);
	clkColorM.addEventListener("change", updateClkColorM, false);

	//  Date
	const datFormat = document.querySelector("#datFormat");
	datFormat.addEventListener("input", updateDatFormat, false);
	const datColor = document.querySelector("#datColor");
	datColor.addEventListener("input", updateDatColor, false);
	const datColorD = document.querySelector("#datColorD");
	datColorD.addEventListener("input", updateDatColorD, false);
	datColorD.addEventListener("change", updateDatColorD, false);
	const datColorM = document.querySelector("#datColorM");
	datColorM.addEventListener("input", updateDatColorM, false);
	datColorM.addEventListener("change", updateDatColorM, false);

	//  Temperature
	const temSymbol = document.querySelector("#temSymbol");
	temSymbol.addEventListener("input", updateTemSymbol, false);

	//  Humidity
	const humColor = document.querySelector("#humColor");
	humColor.addEventListener("input", updateHumColor, false);
	const humColorA = document.querySelector("#humColorA");
	humColorA.addEventListener("input", updateHumColorA, false);
	humColorA.addEventListener("change", updateHumColorA, false);
	const humColorB = document.querySelector("#humColorB");
	humColorB.addEventListener("input", updateHumColorB, false);
	humColorB.addEventListener("change", updateHumColorB, false);
	const humColorC = document.querySelector("#humColorC");
	humColorC.addEventListener("input", updateHumColorC, false);
	humColorC.addEventListener("change", updateHumColorC, false);

	//  Scoreboard
	const scoColorL = document.querySelector("#scoColorL");
	scoColorL.addEventListener("input", updateScoColorL, false);
	scoColorL.addEventListener("change", updateScoColorL, false);
	const scoColorR = document.querySelector("#scoColorR");
	scoColorR.addEventListener("input", updateScoColorR, false);
	scoColorR.addEventListener("change", updateScoColorR, false);

	//  Countdown
	const couColorH = document.querySelector("#couColorH");
	couColorH.addEventListener("input", updateCouColorH, false);
	couColorH.addEventListener("change", updateCouColorH, false);
	const couColorM = document.querySelector("#couColorM");
	couColorM.addEventListener("input", updateCouColorM, false);
	couColorM.addEventListener("change", updateCouColorM, false);
	const couColorS = document.querySelector("#couColorS");
	couColorS.addEventListener("input", updateCouColorS, false);
	couColorS.addEventListener("change", updateCouColorS, false);
	const couColorA = document.querySelector("#couColorA");
	couColorA.addEventListener("input", updateCouColorA, false);
	couColorA.addEventListener("change", updateCouColorA, false);

	//	Scroll
	const scrTime = document.querySelector("#scrTime");
	scrTime.addEventListener("input", updateScrTime, false);

	//  Downlights
	const dowColor = document.querySelector("#dowColor");
	dowColor.addEventListener("input", updateDowColor, false);
	const dowColor0 = document.querySelector("#dowColor0");
	dowColor0.addEventListener("input", updateDowColor0, false);
	dowColor0.addEventListener("change", updateDowColor0, false);
	const dowColor1 = document.querySelector("#dowColor1");
	dowColor1.addEventListener("input", updateDowColor1, false);
	dowColor1.addEventListener("change", updateDowColor1, false);
	const dowColor2 = document.querySelector("#dowColor2");
	dowColor2.addEventListener("input", updateDowColor2, false);
	dowColor2.addEventListener("change", updateDowColor2, false);
	const dowColor3 = document.querySelector("#dowColor3");
	dowColor3.addEventListener("input", updateDowColor3, false);
	dowColor3.addEventListener("change", updateDowColor3, false);
	const dowColor4 = document.querySelector("#dowColor4");
	dowColor4.addEventListener("input", updateDowColor4, false);
	dowColor4.addEventListener("change", updateDowColor4, false);
	const dowColor5 = document.querySelector("#dowColor5");
	dowColor5.addEventListener("input", updateDowColor5, false);
	dowColor5.addEventListener("change", updateDowColor5, false);
	const dowColor6 = document.querySelector("#dowColor6");
	dowColor6.addEventListener("input", updateDowColor6, false);
	dowColor6.addEventListener("change", updateDowColor6, false);
	const dowColor7 = document.querySelector("#dowColor7");
	dowColor7.addEventListener("input", updateDowColor7, false);
	dowColor7.addEventListener("change", updateDowColor7, false);
	const dowColor8 = document.querySelector("#dowColor8");
	dowColor8.addEventListener("input", updateDowColor8, false);
	dowColor8.addEventListener("change", updateDowColor8, false);
	const dowColor9 = document.querySelector("#dowColor9");
	dowColor9.addEventListener("input", updateDowColor9, false);
	dowColor9.addEventListener("change", updateDowColor9, false);
	const dowColor10 = document.querySelector("#dowColor10");
	dowColor10.addEventListener("input", updateDowColor10, false);
	dowColor10.addEventListener("change", updateDowColor10, false);
	const dowColor11 = document.querySelector("#dowColor11");
	dowColor11.addEventListener("input", updateDowColor11, false);
	dowColor11.addEventListener("change", updateDowColor11, false);
	const dowColor12 = document.querySelector("#dowColor12");
	dowColor12.addEventListener("input", updateDowColor12, false);
	dowColor12.addEventListener("change", updateDowColor12, false);
	const dowColor13 = document.querySelector("#dowColor13");
	dowColor13.addEventListener("input", updateDowColor13, false);
	dowColor13.addEventListener("change", updateDowColor13, false);
}

//  ****************************************************************************************************
//  FUNCTIONS - update
//  ****************************************************************************************************
//	Convert HEX color value to RGB color value
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

//  Globale Einstellungen
//  |- Edge
function updateEdgeBrightness(event) {
	const value = event.target.value;
	$.post("/updateEdgeBrightness", { edgeBrightness: value });
	console.log(value);
}
function updateEdgeBrightnessMin(event) {
	const value = event.target.value;
	$.post("/updateEdgeBrightnessMin", { edgeBrightnessMin: value });
	console.log(value);
}
function updateEdgeBrightnessMax(event) {
	const value = event.target.value;
	$.post("/updateEdgeBrightnessMax", { edgeBrightnessMax: value });
	console.log(value);
}
function updateEdgeFrequency(event) {
	const value = event.target.value;
	$.post("/updateEdgeFrequency", { edgeFrequency: value });
	console.log(value);
}
//  |- Downlights
function updateDownlightUsage(event) {
	const value = event.target.value;
	$.post("/updateDownlightUsage", { downUsage: value });
	console.log(value);
}
function updateDownlightBrightness(event) {
	const value = event.target.value;
	$.post("/updateDownlightBrightness", { downBrightness: value });
	console.log(value);
}
function updateDownlightBrightnessMin(event) {
	const value = event.target.value;
	$.post("/updateDownlightBrightnessMin", { downBrightnessMin: value });
	console.log(value);
}
function updateDownlightBrightnessMax(event) {
	const value = event.target.value;
	$.post("/updateDownlightBrightnessMax", { downBrightnessMax: value });
	console.log(value);
}
function updateDownlightFrequency(event) {
	const value = event.target.value;
	$.post("/updateDownlightFrequency", { downFrequency: value });
	console.log(value);
}

//  KNX
function updateKNXAddress(event) {
	const value = event.target.value;
	$.post("/updateKNXAddress", { knxAddress: value });
	console.log(value);
}
function updateKNXPort(event) {
	const value = event.target.value;
	$.post("/updateKNXPort", { knxPort: value });
	console.log(value);
}
function updateKNXBrightnessPath(event) {
	const value = event.target.value;
	$.post("/updateKNXBrightnessPath", { knxBrightnessPath: value });
	console.log(value);
}
function updateKNXBrightnessMin(event) {
	const value = event.target.value;
	$.post("/updateKNXBrightnessMin", { knxBrightnessMin: value });
	console.log(value);
}
function updateKNXBrightnessMax(event) {
	const value = event.target.value;
	$.post("/updateKNXBrightnessMax", { knxBrightnessMax: value });
	console.log(value);
}
function updateKNXTemperaturePath(event) {
	const value = event.target.value;
	$.post("/updateKNXTemperaturePath", { knxTemperaturePath: value });
	console.log(value);
}
function updateKNXHumidityPath(event) {
	const value = event.target.value;
	$.post("/updateKNXHumidityPath", { knxHumidityPath: value });
	console.log(value);
}

//  Clock
function updateClkAddress(event) {
	const value = event.target.value;
	$.post("/updateClkAddress", { clkAddress: value });
	console.log(value);
}
function updateClkTimeZone(event) {
	const value = event.target.value;
	$.post("/updateClkTimeZone", { clkTimeZone: value });
	console.log(value);
}
function updateClkFormat(event) {
	const value = event.target.value;
	$.post("/updateClkFormat", { clkFormat: value });
	console.log(value);
}
function updateClkColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("clkColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("clkColorInput").hidden = true;
	}
	$.post("/updateClkColor", { clkColor: value });
	console.log(value);
}
function updateClkColorH(event) {
	const value = event.target.value;
	$.post("/updateClkColorH", hexToRgb(value));
	closest("#clkColorH", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateClkColorM(event) {
	const value = event.target.value;
	$.post("/updateClkColorM", hexToRgb(value));
	closest("#clkColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Date
function updateDatFormat(event) {
	const value = event.target.value;
	$.post("/updateDatFormat", { datFormat: value });
	console.log(value);
}
function updateDatColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("datColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("datColorInput").hidden = true;
	}

	$.post("/updateDatColor", { datColor: value });
	console.log(value);
}
function updateDatColorD(event) {
	const value = event.target.value;
	$.post("/updateDatColorD", hexToRgb(value));
	closest("#datColorD", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDatColorM(event) {
	const value = event.target.value;
	$.post("/updateDatColorM", hexToRgb(value));
	closest("#datColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Temperature
function updateTemSymbol(event) {
	const value = event.target.value;
	$.post("/updateTemSymbol", { temSymbol: value });
	console.log(value);
}

//  Humidity
function updateHumColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("humColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("humColorInput").hidden = true;
	}
	$.post("/updateHumColor", { humColor: value });
	console.log(value);
}
function updateHumColorA(event) {
	const value = event.target.value;
	$.post("/updateHumColorA", hexToRgb(value));
	closest("#humColorA", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateHumColorB(event) {
	const value = event.target.value;
	$.post("/updateHumColorB", hexToRgb(value));
	closest("#humColorB", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateHumColorC(event) {
	const value = event.target.value;
	$.post("/updateHumColorC", hexToRgb(value));
	closest("#humColorC", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Scoreboard
function updateScoColorL(event) {
	const value = event.target.value;
	$.post("/updateScoColorL", hexToRgb(value));
	closest("#scoColorL", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateScoColorR(event) {
	const value = event.target.value;
	$.post("/updateScoColorR", hexToRgb(value));
	closest("#scoColorR", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Countdown
function updateCouColorH(event) {
	const value = event.target.value;
	$.post("/updateCouColorH", hexToRgb(value));
	closest("#couColorH", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateCouColorM(event) {
	const value = event.target.value;
	$.post("/updateCouColorM", hexToRgb(value));
	closest("#couColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateCouColorS(event) {
	const value = event.target.value;
	$.post("/updateCouColorS", hexToRgb(value));
	closest("#couColorS", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateCouColorA(event) {
	const value = event.target.value;
	$.post("/updateCouColorH", hexToRgb(value));
	closest("#couColorA", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

// 	Scroll
function updateScrTime(event) {
	const value = event.target.value;
	$.post("/updateScrTime", { scrTime: value });
	console.log(value);
}

//  Downlights
function updateDowColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("dowColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("dowColorInput").hidden = true;
	}
	if (value === "2") {
		document.getElementById("dowColorInput").hidden = true;
	}
	$.post("/updateDowColor", { dowColor: value });
	console.log(value);
}
function updateDowColor0(event) {
	const value = event.target.value;
	$.post("/updateDowColor0", hexToRgb(value));
	closest("#dowColor0", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor1(event) {
	const value = event.target.value;
	$.post("/updateDowColor1", hexToRgb(value));
	closest("#dowColor1", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor2(event) {
	const value = event.target.value;
	$.post("/updateDowColor2", hexToRgb(value));
	closest("#dowColor2", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor3(event) {
	const value = event.target.value;
	$.post("/updateDowColor3", hexToRgb(value));
	closest("#dowColor3", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor4(event) {
	const value = event.target.value;
	$.post("/updateDowColor4", hexToRgb(value));
	closest("#dowColor4", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor5(event) {
	const value = event.target.value;
	$.post("/updateDowColor5", hexToRgb(value));
	closest("#dowColor5", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor6(event) {
	const value = event.target.value;
	$.post("/updateDowColor6", hexToRgb(value));
	closest("#dowColor6", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor7(event) {
	const value = event.target.value;
	$.post("/updateDowColor7", hexToRgb(value));
	closest("#dowColor7", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor8(event) {
	const value = event.target.value;
	$.post("/updateDowColor8", hexToRgb(value));
	closest("#dowColor8", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor9(event) {
	const value = event.target.value;
	$.post("/updateDowColor9", hexToRgb(value));
	closest("#dowColor9", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor10(event) {
	const value = event.target.value;
	$.post("/updateDowColor10", hexToRgb(value));
	closest("#dowColor10", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor11(event) {
	const value = event.target.value;
	$.post("/updateDowColor11", hexToRgb(value));
	closest("#dowColor11", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor12(event) {
	const value = event.target.value;
	$.post("/updateDowColor12", hexToRgb(value));
	closest("#dowColor12", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updateDowColor13(event) {
	const value = event.target.value;
	$.post("/updateDowColor13", hexToRgb(value));
	closest("#dowColor13", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
