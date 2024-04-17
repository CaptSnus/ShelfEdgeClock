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
function getEdgeBrightIn() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("edgeBrightIn").selectedIndex = "0";
				document.getElementById("edgeBrightFields").hidden = false;
			}
			if (this.responseText === "25") {
				document.getElementById("edgeBrightIn").selectedIndex = "1";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "50") {
				document.getElementById("edgeBrightIn").selectedIndex = "2";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "75") {
				document.getElementById("edgeBrightIn").selectedIndex = "3";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "100") {
				document.getElementById("edgeBrightIn").selectedIndex = "4";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "125") {
				document.getElementById("edgeBrightIn").selectedIndex = "5";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "150") {
				document.getElementById("edgeBrightIn").selectedIndex = "6";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "175") {
				document.getElementById("edgeBrightIn").selectedIndex = "7";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "200") {
				document.getElementById("edgeBrightIn").selectedIndex = "8";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "225") {
				document.getElementById("edgeBrightIn").selectedIndex = "9";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "250") {
				document.getElementById("edgeBrightIn").selectedIndex = "10";
				document.getElementById("edgeBrightFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getEdgeBrightIn", true);
	xhttp.send();
}
function getEdgeBrightMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("edgeBrightMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getEdgeBrightMin", true);
	xhttp.send();
}
function getEdgeBrightMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("edgeBrightMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getEdgeBrightMax", true);
	xhttp.send();
}
function getEdgeBrightOut() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("edgeBrightOut").value = this.responseText;
		}
	};
	xhttp.open("GET", "getEdgeBrightOut", true);
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
function getDownSwitch() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downSwitch").selectedIndex = "0";
				document.getElementById("downFields").hidden = true;
			}
			if (this.responseText === "1") {
				document.getElementById("downSwitch").selectedIndex = "1";
				document.getElementById("downFields").hidden = false;
			}
		}
	};
	xhttp.open("GET", "getDownSwitch", true);
	xhttp.send();
}
function getDownBrightIn() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downBrightIn").selectedIndex = "0";
				document.getElementById("edgeBrightFields").hidden = false;
			}
			if (this.responseText === "25") {
				document.getElementById("downBrightIn").selectedIndex = "1";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "50") {
				document.getElementById("downBrightIn").selectedIndex = "2";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "75") {
				document.getElementById("downBrightIn").selectedIndex = "3";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "100") {
				document.getElementById("downBrightIn").selectedIndex = "4";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "125") {
				document.getElementById("downBrightIn").selectedIndex = "5";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "150") {
				document.getElementById("downBrightIn").selectedIndex = "6";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "175") {
				document.getElementById("downBrightIn").selectedIndex = "7";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "200") {
				document.getElementById("downBrightIn").selectedIndex = "8";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "225") {
				document.getElementById("downBrightIn").selectedIndex = "9";
				document.getElementById("edgeBrightFields").hidden = true;
			}
			if (this.responseText === "250") {
				document.getElementById("downBrightIn").selectedIndex = "10";
				document.getElementById("edgeBrightFields").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getDownBrightIn", true);
	xhttp.send();
	getDownBrightOut();
}
function getDownBrightMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downBrightMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getDownBrightMin", true);
	xhttp.send();
}
function getDownBrightMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downBrightMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getDownBrightMax", true);
	xhttp.send();
}
function getDownBrightOut() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downBrightOut").value = this.responseText;
		}
	};
	xhttp.open("GET", "getDownBrightOut", true);
	xhttp.send();
}
function getDownFrequency() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downFrequency").selectedIndex = "0";
			}
			if (this.responseText === "1") {
				document.getElementById("downFrequency").selectedIndex = "1";
			}
			if (this.responseText === "2") {
				document.getElementById("downFrequency").selectedIndex = "2";
			}
			if (this.responseText === "3") {
				document.getElementById("downFrequency").selectedIndex = "3";
			}
		}
	};
	xhttp.open("GET", "getDownFrequency", true);
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
function getKNXBrightPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightPath", true);
	xhttp.send();
}
function getKNXBrightMin() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightMin").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightMin", true);
	xhttp.send();
}
function getKNXBrightMax() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxBrightMax").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXBrightMax", true);
	xhttp.send();
}
function getKNXTempPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxTempPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXTempPath", true);
	xhttp.send();
}
function getKNXHumidPath() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("knxHumidPath").value = this.responseText;
		}
	};
	xhttp.open("GET", "getKNXHumidPath", true);
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
			closest("#clkColorH", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#clkColorM", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#datColorD", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#datColorM", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#humColorA", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#humColorB", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#humColorC", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#scoColorL", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#scoColorR", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#couColorH", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#couColorM", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#couColorS", ".input-group-text").style.backgroundColor =
				this.responseText;
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
			closest("#couColorA", ".input-group-text").style.backgroundColor =
				this.responseText;
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
function getDownColorMode() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("downColorMode").selectedIndex = 0;
				document.getElementById("downColorInput0").hidden = false;
				document.getElementById("downColorInput1").hidden = true;
			}
			if (this.responseText === "1") {
				document.getElementById("downColorMode").selectedIndex = 1;
				document.getElementById("downColorInput0").hidden = true;
				document.getElementById("downColorInput1").hidden = false;
			}
			if (this.responseText === "2") {
				document.getElementById("downColorMode").selectedIndex = 2;
				document.getElementById("downColorInput0").hidden = true;
				document.getElementById("downColorInput1").hidden = true;
			}
			if (this.responseText === "3") {
				document.getElementById("downColorMode").selectedIndex = 3;
				document.getElementById("downColorInput0").hidden = true;
				document.getElementById("downColorInput1").hidden = true;
			}
		}
	};
	xhttp.open("GET", "getDownColorMode", true);
	xhttp.send();
}
function getDownColor00() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor00").value = this.responseText;
			closest("#downColor00", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor00", true);
	xhttp.send();
}
function getDownColor01() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor01").value = this.responseText;
			closest("#downColor01", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor01", true);
	xhttp.send();
}
function getDownColor02() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor02").value = this.responseText;
			closest("#downColor02", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor02", true);
	xhttp.send();
}
function getDownColor03() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor03").value = this.responseText;
			closest("#downColor03", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor03", true);
	xhttp.send();
}
function getDownColor04() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor04").value = this.responseText;
			closest("#downColor04", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor04", true);
	xhttp.send();
}
function getDownColor05() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor05").value = this.responseText;
			closest("#downColor05", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor05", true);
	xhttp.send();
}
function getDownColor06() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor06").value = this.responseText;
			closest("#downColor06", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor06", true);
	xhttp.send();
}
function getDownColor07() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor07").value = this.responseText;
			closest("#downColor07", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor07", true);
	xhttp.send();
}
function getDownColor08() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor08").value = this.responseText;
			closest("#downColor08", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor08", true);
	xhttp.send();
}
function getDownColor09() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor09").value = this.responseText;
			closest("#downColor09", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor09", true);
	xhttp.send();
}
function getDownColor10() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor10").value = this.responseText;
			closest("#downColor10", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor10", true);
	xhttp.send();
}
function getDownColor11() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor11").value = this.responseText;
			closest("#downColor11", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor11", true);
	xhttp.send();
}
function getDownColor12() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor12").value = this.responseText;
			closest("#downColor12", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor12", true);
	xhttp.send();
}
function getDownColor13() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor13").value = this.responseText;
			closest("#downColor13", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor13", true);
	xhttp.send();
}
function getDownColor14() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("downColor14").value = this.responseText;
			closest("#downColor14", ".input-group-text").style.backgroundColor =
				this.responseText;
		}
	};
	xhttp.open("GET", "getDownColor14", true);
	xhttp.send();
}

//  ****************************************************************************************************
//  EVENT LISTENER - get
//  ****************************************************************************************************
function init() {
	//  Globale Einstellungen
	//  |- Edge
	getEdgeBrightIn();
	getEdgeBrightMin();
	getEdgeBrightMax();
	getEdgeBrightOut();
	getEdgeFrequency();
	//  |- Downlights
	getDownSwitch();
	getDownBrightIn();
	getDownBrightMin();
	getDownBrightMax();
	getDownBrightOut();
	getDownFrequency();

	//  KNX
	getKNXAddress();
	getKNXPort();
	getKNXBrightPath();
	getKNXBrightMin();
	getKNXBrightMax();
	getKNXTempPath();
	getKNXHumidPath();

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
	getDownColorMode();
	getDownColor00();
	getDownColor01();
	getDownColor02();
	getDownColor03();
	getDownColor04();
	getDownColor05();
	getDownColor06();
	getDownColor07();
	getDownColor08();
	getDownColor09();
	getDownColor10();
	getDownColor11();
	getDownColor12();
	getDownColor13();
	getDownColor14();
}

//  ****************************************************************************************************
//  EVENT LISTENER - update
//  ****************************************************************************************************
function startup() {
	//  Globale Einstellungen
	//  |- Edge
	const edgeBrightIn = document.querySelector("#edgeBrightIn");
	edgeBrightIn.addEventListener("input", updEdgeBrightIn, false);
	const edgeBrightMin = document.querySelector("#edgeBrightMin");
	edgeBrightMin.addEventListener("input", updEdgeBrightMin, false);
	const edgeBrightMax = document.querySelector("#edgeBrightMax");
	edgeBrightMax.addEventListener("input", updEdgeBrightMax, false);
	const edgeFrequency = document.querySelector("#edgeFrequency");
	edgeFrequency.addEventListener("input", updEdgeFrequency, false);
	//  |- Downlights
	const downSwitch = document.querySelector("#downSwitch");
	downSwitch.addEventListener("input", updDownSwitch, false);
	const downBrightIn = document.querySelector("#downBrightIn");
	downBrightIn.addEventListener("input", updDownBrightIn, false);
	const downBrightMin = document.querySelector("#downBrightMin");
	downBrightMin.addEventListener("input", updDownBrightMin, false);
	const downBrightMax = document.querySelector("#downBrightMax");
	downBrightMax.addEventListener("input", updDownBrightMax, false);
	const downFrequency = document.querySelector("#downFrequency");
	downFrequency.addEventListener("input", updDownFrequency, false);

	// KNX
	const knxAddress = document.querySelector("#knxAddress");
	knxAddress.addEventListener("input", updKNXAddress, false);
	const knxPort = document.querySelector("#knxPort");
	knxPort.addEventListener("input", updKNXPort, false);
	const knxBrightPath = document.querySelector("#knxBrightPath");
	knxBrightPath.addEventListener("input", updKNXBrightPath, false);
	const knxBrightMin = document.querySelector("#knxBrightMin");
	knxBrightMin.addEventListener("input", updKNXBrightMin, false);
	const knxBrightMax = document.querySelector("#knxBrightMax");
	knxBrightMax.addEventListener("input", updKNXBrightMax, false);
	const knxTempPath = document.querySelector("#knxTempPath");
	knxTempPath.addEventListener("input", updKNXTempPath, false);
	const knxHumidPath = document.querySelector("#knxHumidPath");
	knxHumidPath.addEventListener("input", updKNXHumidPath, false);

	//  Clock
	const clkAddress = document.querySelector("#clkAddress");
	clkAddress.addEventListener("input", updClkAddress, false);
	const clkTimeZone = document.querySelector("#clkTimeZone");
	clkTimeZone.addEventListener("input", updClkTimeZone, false);
	const clkFormat = document.querySelector("#clkFormat");
	clkFormat.addEventListener("input", updClkFormat, false);
	const clkColor = document.querySelector("#clkColor");
	clkColor.addEventListener("input", updClkColor, false);
	const clkColorH = document.querySelector("#clkColorH");
	clkColorH.addEventListener("input", updClkColorH, false);
	clkColorH.addEventListener("change", updClkColorH, false);
	const clkColorM = document.querySelector("#clkColorM");
	clkColorM.addEventListener("input", updClkColorM, false);
	clkColorM.addEventListener("change", updClkColorM, false);

	//  Date
	const datFormat = document.querySelector("#datFormat");
	datFormat.addEventListener("input", updDatFormat, false);
	const datColor = document.querySelector("#datColor");
	datColor.addEventListener("input", updDatColor, false);
	const datColorD = document.querySelector("#datColorD");
	datColorD.addEventListener("input", updDatColorD, false);
	datColorD.addEventListener("change", updDatColorD, false);
	const datColorM = document.querySelector("#datColorM");
	datColorM.addEventListener("input", updDatColorM, false);
	datColorM.addEventListener("change", updDatColorM, false);

	//  Temperature
	const temSymbol = document.querySelector("#temSymbol");
	temSymbol.addEventListener("input", updTemSymbol, false);

	//  Humidity
	const humColor = document.querySelector("#humColor");
	humColor.addEventListener("input", updHumColor, false);
	const humColorA = document.querySelector("#humColorA");
	humColorA.addEventListener("input", updHumColorA, false);
	humColorA.addEventListener("change", updHumColorA, false);
	const humColorB = document.querySelector("#humColorB");
	humColorB.addEventListener("input", updHumColorB, false);
	humColorB.addEventListener("change", updHumColorB, false);
	const humColorC = document.querySelector("#humColorC");
	humColorC.addEventListener("input", updHumColorC, false);
	humColorC.addEventListener("change", updHumColorC, false);

	//  Scoreboard
	const scoColorL = document.querySelector("#scoColorL");
	scoColorL.addEventListener("input", updScoColorL, false);
	scoColorL.addEventListener("change", updScoColorL, false);
	const scoColorR = document.querySelector("#scoColorR");
	scoColorR.addEventListener("input", updScoColorR, false);
	scoColorR.addEventListener("change", updScoColorR, false);

	//  Countdown
	const couColorH = document.querySelector("#couColorH");
	couColorH.addEventListener("input", updCouColorH, false);
	couColorH.addEventListener("change", updCouColorH, false);
	const couColorM = document.querySelector("#couColorM");
	couColorM.addEventListener("input", updCouColorM, false);
	couColorM.addEventListener("change", updCouColorM, false);
	const couColorS = document.querySelector("#couColorS");
	couColorS.addEventListener("input", updCouColorS, false);
	couColorS.addEventListener("change", updCouColorS, false);
	const couColorA = document.querySelector("#couColorA");
	couColorA.addEventListener("input", updCouColorA, false);
	couColorA.addEventListener("change", updCouColorA, false);

	//	Scroll
	const scrTime = document.querySelector("#scrTime");
	scrTime.addEventListener("input", updScrTime, false);

	//  Downlights
	const downColorMode = document.querySelector("#downColorMode");
	downColorMode.addEventListener("input", updDownColorMode, false);
	const downColor00 = document.querySelector("#downColor00");
	downColor00.addEventListener("input", updDownColor00, false);
	downColor00.addEventListener("change", updDownColor00, false);
	const downColor01 = document.querySelector("#downColor01");
	downColor01.addEventListener("input", updDownColor01, false);
	downColor01.addEventListener("change", updDownColor01, false);
	const downColor02 = document.querySelector("#downColor02");
	downColor02.addEventListener("input", updDownColor02, false);
	downColor02.addEventListener("change", updDownColor02, false);
	const downColor03 = document.querySelector("#downColor03");
	downColor03.addEventListener("input", updDownColor03, false);
	downColor03.addEventListener("change", updDownColor03, false);
	const downColor04 = document.querySelector("#downColor04");
	downColor04.addEventListener("input", updDownColor04, false);
	downColor04.addEventListener("change", updDownColor04, false);
	const downColor05 = document.querySelector("#downColor05");
	downColor05.addEventListener("input", updDownColor05, false);
	downColor05.addEventListener("change", updDownColor05, false);
	const downColor06 = document.querySelector("#downColor06");
	downColor06.addEventListener("input", updDownColor06, false);
	downColor06.addEventListener("change", updDownColor06, false);
	const downColor07 = document.querySelector("#downColor07");
	downColor07.addEventListener("input", updDownColor07, false);
	downColor07.addEventListener("change", updDownColor07, false);
	const downColor08 = document.querySelector("#downColor08");
	downColor08.addEventListener("input", updDownColor08, false);
	downColor08.addEventListener("change", updDownColor08, false);
	const downColor09 = document.querySelector("#downColor09");
	downColor09.addEventListener("input", updDownColor09, false);
	downColor09.addEventListener("change", updDownColor09, false);
	const downColor10 = document.querySelector("#downColor10");
	downColor10.addEventListener("input", updDownColor10, false);
	downColor10.addEventListener("change", updDownColor10, false);
	const downColor11 = document.querySelector("#downColor11");
	downColor11.addEventListener("input", updDownColor11, false);
	downColor11.addEventListener("change", updDownColor11, false);
	const downColor12 = document.querySelector("#downColor12");
	downColor12.addEventListener("input", updDownColor12, false);
	downColor12.addEventListener("change", updDownColor12, false);
	const downColor13 = document.querySelector("#downColor13");
	downColor13.addEventListener("input", updDownColor13, false);
	downColor13.addEventListener("change", updDownColor13, false);
	const downColor14 = document.querySelector("#downColor14");
	downColor14.addEventListener("input", updDownColor14, false);
	downColor14.addEventListener("change", updDownColor14, false);
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
function updEdgeBrightIn(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("edgeBrightFields").hidden = false;
	} else {
		document.getElementById("edgeBrightFields").hidden = true;
	}
	$.post("/updEdgeBrightIn", { edgeBrightIn: value });
	console.log(value);
}
function updEdgeBrightMin(event) {
	const value = event.target.value;
	$.post("/updEdgeBrightMin", { edgeBrightMin: value });
	console.log(value);
}
function updEdgeBrightMax(event) {
	const value = event.target.value;
	$.post("/updEdgeBrightMax", { edgeBrightMax: value });
	console.log(value);
}
function updEdgeFrequency(event) {
	const value = event.target.value;
	$.post("/updEdgeFrequency", { edgeFrequency: value });
	console.log(value);
}
//  |- Downlights
function updDownSwitch(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("downFields").hidden = true;
	} else {
		document.getElementById("downFields").hidden = false;
	}
	$.post("/updDownSwitch", { downSwitch: value });
	console.log(value);
}
function updDownBrightIn(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("downBrightFields").hidden = false;
	} else {
		document.getElementById("downBrightFields").hidden = true;
	}
	$.post("/updDownBrightIn", { downBrightIn: value });
	console.log(value);
}
function updDownBrightMin(event) {
	const value = event.target.value;
	$.post("/updDownBrightMin", { downBrightMin: value });
	console.log(value);
}
function updDownBrightMax(event) {
	const value = event.target.value;
	$.post("/updDownBrightMax", { downBrightMax: value });
	console.log(value);
}
function updDownFrequency(event) {
	const value = event.target.value;
	$.post("/updDownFrequency", { downFrequency: value });
	console.log(value);
}

//  KNX
function updKNXAddress(event) {
	const value = event.target.value;
	$.post("/updKNXAddress", { knxAddress: value });
	console.log(value);
}
function updKNXPort(event) {
	const value = event.target.value;
	$.post("/updKNXPort", { knxPort: value });
	console.log(value);
}
function updKNXBrightPath(event) {
	const value = event.target.value;
	$.post("/updKNXBrightPath", { knxBrightPath: value });
	console.log(value);
}
function updKNXBrightMin(event) {
	const value = event.target.value;
	$.post("/updKNXBrightMin", { knxBrightMin: value });
	console.log(value);
}
function updKNXBrightMax(event) {
	const value = event.target.value;
	$.post("/updKNXBrightMax", { knxBrightMax: value });
	console.log(value);
}
function updKNXTempPath(event) {
	const value = event.target.value;
	$.post("/updKNXTempPath", { knxTempPath: value });
	console.log(value);
}
function updKNXHumidPath(event) {
	const value = event.target.value;
	$.post("/updKNXHumidPath", { knxHumidPath: value });
	console.log(value);
}

//  Clock
function updClkAddress(event) {
	const value = event.target.value;
	$.post("/updClkAddress", { clkAddress: value });
	console.log(value);
}
function updClkTimeZone(event) {
	const value = event.target.value;
	$.post("/updClkTimeZone", { clkTimeZone: value });
	console.log(value);
}
function updClkFormat(event) {
	const value = event.target.value;
	$.post("/updClkFormat", { clkFormat: value });
	console.log(value);
}
function updClkColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("clkColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("clkColorInput").hidden = true;
	}
	$.post("/updClkColor", { clkColor: value });
	console.log(value);
}
function updClkColorH(event) {
	const value = event.target.value;
	$.post("/updClkColorH", hexToRgb(value));
	closest("#clkColorH", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updClkColorM(event) {
	const value = event.target.value;
	$.post("/updClkColorM", hexToRgb(value));
	closest("#clkColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Date
function updDatFormat(event) {
	const value = event.target.value;
	$.post("/updDatFormat", { datFormat: value });
	console.log(value);
}
function updDatColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("datColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("datColorInput").hidden = true;
	}

	$.post("/updDatColor", { datColor: value });
	console.log(value);
}
function updDatColorD(event) {
	const value = event.target.value;
	$.post("/updDatColorD", hexToRgb(value));
	closest("#datColorD", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDatColorM(event) {
	const value = event.target.value;
	$.post("/updDatColorM", hexToRgb(value));
	closest("#datColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Temperature
function updTemSymbol(event) {
	const value = event.target.value;
	$.post("/updTemSymbol", { temSymbol: value });
	console.log(value);
}

//  Humidity
function updHumColor(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("humColorInput").hidden = false;
	}
	if (value === "1") {
		document.getElementById("humColorInput").hidden = true;
	}
	$.post("/updHumColor", { humColor: value });
	console.log(value);
}
function updHumColorA(event) {
	const value = event.target.value;
	$.post("/updHumColorA", hexToRgb(value));
	closest("#humColorA", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updHumColorB(event) {
	const value = event.target.value;
	$.post("/updHumColorB", hexToRgb(value));
	closest("#humColorB", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updHumColorC(event) {
	const value = event.target.value;
	$.post("/updHumColorC", hexToRgb(value));
	closest("#humColorC", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Scoreboard
function updScoColorL(event) {
	const value = event.target.value;
	$.post("/updScoColorL", hexToRgb(value));
	closest("#scoColorL", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updScoColorR(event) {
	const value = event.target.value;
	$.post("/updScoColorR", hexToRgb(value));
	closest("#scoColorR", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

//  Countdown
function updCouColorH(event) {
	const value = event.target.value;
	$.post("/updCouColorH", hexToRgb(value));
	closest("#couColorH", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updCouColorM(event) {
	const value = event.target.value;
	$.post("/updCouColorM", hexToRgb(value));
	closest("#couColorM", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updCouColorS(event) {
	const value = event.target.value;
	$.post("/updCouColorS", hexToRgb(value));
	closest("#couColorS", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updCouColorA(event) {
	const value = event.target.value;
	$.post("/updCouColorH", hexToRgb(value));
	closest("#couColorA", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}

// 	Scroll
function updScrTime(event) {
	const value = event.target.value;
	$.post("/updScrTime", { scrTime: value });
	console.log(value);
}

//  Downlights
function updDownColorMode(event) {
	const value = event.target.value;
	if (value === "0") {
		document.getElementById("downColorInput0").hidden = false;
		document.getElementById("downColorInput1").hidden = true;
	}
	if (value === "1") {
		document.getElementById("downColorInput0").hidden = true;
		document.getElementById("downColorInput1").hidden = false;
	}
	if (value === "2") {
		document.getElementById("downColorInput0").hidden = true;
		document.getElementById("downColorInput1").hidden = true;
	}
	if (value === "3") {
		document.getElementById("downColorInput0").hidden = true;
		document.getElementById("downColorInput1").hidden = true;
	}
	$.post("/updDownColorMode", { downColorMode: value });
	console.log(value);
}
function updDownColor00(event) {
	const value = event.target.value;
	$.post("/updDownColor00", hexToRgb(value));
	closest("#downColor00", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor01(event) {
	const value = event.target.value;
	$.post("/updDownColor01", hexToRgb(value));
	closest("#downColor01", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor02(event) {
	const value = event.target.value;
	$.post("/updDownColor02", hexToRgb(value));
	closest("#downColor02", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor03(event) {
	const value = event.target.value;
	$.post("/updDownColor03", hexToRgb(value));
	closest("#downColor03", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor04(event) {
	const value = event.target.value;
	$.post("/updDownColor04", hexToRgb(value));
	closest("#downColor04", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor05(event) {
	const value = event.target.value;
	$.post("/updDownColor05", hexToRgb(value));
	closest("#downColor05", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor06(event) {
	const value = event.target.value;
	$.post("/updDownColor06", hexToRgb(value));
	closest("#downColor06", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor07(event) {
	const value = event.target.value;
	$.post("/updDownColor07", hexToRgb(value));
	closest("#downColor07", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor08(event) {
	const value = event.target.value;
	$.post("/updDownColor08", hexToRgb(value));
	closest("#downColor08", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor09(event) {
	const value = event.target.value;
	$.post("/updDownColor09", hexToRgb(value));
	closest("#downColor09", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor10(event) {
	const value = event.target.value;
	$.post("/updDownColor10", hexToRgb(value));
	closest("#downColor10", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor11(event) {
	const value = event.target.value;
	$.post("/updDownColor11", hexToRgb(value));
	closest("#downColor11", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor12(event) {
	const value = event.target.value;
	$.post("/updDownColor12", hexToRgb(value));
	closest("#downColor12", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor13(event) {
	const value = event.target.value;
	$.post("/updDownColor13", hexToRgb(value));
	closest("#downColor13", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
function updDownColor14(event) {
	const value = event.target.value;
	$.post("/updDownColor14", hexToRgb(value));
	closest("#downColor14", ".input-group-text").style.backgroundColor = value;
	console.log(hexToRgb(value));
}
