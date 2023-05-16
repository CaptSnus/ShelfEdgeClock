// Variables

//  NETZWERK
//  |-  ACCESS POINT
let apConfigValue;
let apSSIDValue;
let apPassValue;
let apPassError;
let apHideValue;
let apIP1Value;
let apIP1Error;
let apIP2Value;
let apIP2Error;
let apIP3Value;
let apIP3Error;
let apIP4Value;
let apIP4Error;
let apGW1Value;
let apGW2Value;
let apGW3Value;
let apGW4Value;
let apSN1Value;
let apSN2Value;
let apSN3Value;
let apSN4Value;
//  |-  WIFI CLIENT
let wifiConfigValue;
let wifiSSIDValue;
let wifiPassValue;
let wifiDHCPValue;
let wifiIP1Value;
let wifiIP2Value;
let wifiIP3Value;
let wifiIP4Value;
let wifiGW1Value;
let wifiGW2Value;
let wifiGW3Value;
let wifiGW4Value;
let wifiSN1Value;
let wifiSN2Value;
let wifiSN3Value;
let wifiSN4Value;

//  ****************************************************************************************************
//  FUNCTIONS - get
//  ****************************************************************************************************
//  NETWORK
//  |-  ACCESS POINT
function getAPConfig() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("apConfig").checked = false;
				apConfigValue = false;
				unLockAPFields();
			}
			if (this.responseText === "1") {
				document.getElementById("apConfig").checked = true;
				apConfigValue = true;
				unLockAPFields();
			}
		}
	};
	xhttp.open("GET", "getAPConfig", true);
	xhttp.send();
}
function getAPSSID() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSSID").value = this.responseText;
		}
	};
	xhttp.open("GET", "getAPSSID", true);
	xhttp.send();
}
function getAPPass() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apPass").value = this.responseText;
			apPassValue = apPass.value;
			validatePass();
		}
	};
	xhttp.open("GET", "getAPPass", true);
	xhttp.send();
}
function getAPHide() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apHide").value = this.responseText;
		}
	};
	xhttp.open("GET", "getAPHide", true);
	xhttp.send();
}
function getAPIP1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apIP1").value = this.responseText;
			apIP1Value = apIP1.valueAsNumber;
			validateIP();
		}
	};
	xhttp.open("GET", "getAPIP1", true);
	xhttp.send();
}
function getAPIP2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apIP2").value = this.responseText;
			apIP2Value = apIP2.valueAsNumber;
			validateIP();
		}
	};
	xhttp.open("GET", "getAPIP2", true);
	xhttp.send();
}
function getAPIP3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apIP3").value = this.responseText;
			apIP3Value = apIP3.valueAsNumber;
			validateIP();
		}
	};
	xhttp.open("GET", "getAPIP3", true);
	xhttp.send();
}
function getAPIP4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apIP4").value = this.responseText;
			apIP4Value = apIP4.valueAsNumber;
			validateIP();
		}
	};
	xhttp.open("GET", "getAPIP4", true);
	xhttp.send();
}
function getAPGW1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apGW1").value = this.responseText;
			apGW1Value = apGW1.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPGW1", true);
	xhttp.send();
}
function getAPGW2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apGW2").value = this.responseText;
			apGW2Value = apGW2.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPGW2", true);
	xhttp.send();
}
function getAPGW3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apGW3").value = this.responseText;
			apGW3Value = apGW3.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPGW3", true);
	xhttp.send();
}
function getAPGW4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apGW4").value = this.responseText;
			apGW4Value = apGW4.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPGW4", true);
	xhttp.send();
}
function getAPSN1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSN1").value = this.responseText;
			apSN1Value = apSN1.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPSN1", true);
	xhttp.send();
}
function getAPSN2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSN2").value = this.responseText;
			apSN2Value = apSN2.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPSN2", true);
	xhttp.send();
}
function getAPSN3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSN3").value = this.responseText;
			apSN3Value = apSN3.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPSN3", true);
	xhttp.send();
}
function getAPSN4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSN4").value = this.responseText;
			apSN4Value = apSN4.valueAsNumber;
		}
	};
	xhttp.open("GET", "getAPSN4", true);
	xhttp.send();
}
//  |-  WIFI CLIENT
function getWifiConfig() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("wifiConfig").checked = false;
				wifiConfigValue = false;
				unLockWifiFields();
			}
			if (this.responseText === "1") {
				document.getElementById("wifiConfig").checked = true;
				wifiConfigValue = true;
				unLockWifiFields();
			}
		}
	};
	xhttp.open("GET", "getWifiConfig", true);
	xhttp.send();
}
function getWifiSSID() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiSSID").value = this.responseText;
		}
	};
	xhttp.open("GET", "getWifiSSID", true);
	xhttp.send();
}
function getWifiPass() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiPass").value = this.responseText;
		}
	};
	xhttp.open("GET", "getWifiPass", true);
	xhttp.send();
}
function getWifiDHCP() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiDHCP").value = this.responseText;
			wifiDHCPValue = this.responseText;
			unlockDHCPFields();
		}
	};
	xhttp.open("GET", "getWifiDHCP", true);
	xhttp.send();
}
function getWifiIP1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiIP1").value = this.responseText;
			wifiIP1Value = wifiIP1.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiIP1", true);
	xhttp.send();
}
function getWifiIP2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiIP2").value = this.responseText;
			wifiIP2Value = wifiIP2.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiIP2", true);
	xhttp.send();
}
function getWifiIP3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiIP3").value = this.responseText;
			wifiIP3Value = wifiIP3.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiIP3", true);
	xhttp.send();
}
function getWifiIP4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiIP4").value = this.responseText;
			wifiIP4Value = wifiIP4.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiIP4", true);
	xhttp.send();
}
function getWifiGW1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiGW1").value = this.responseText;
			wifiGW1Value = wifiGW1.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiGW1", true);
	xhttp.send();
}
function getWifiGW2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiGW2").value = this.responseText;
			wifiGW2Value = wifiGW2.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiGW2", true);
	xhttp.send();
}
function getWifiGW3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiGW3").value = this.responseText;
			wifiGW3Value = wifiGW3.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiGW3", true);
	xhttp.send();
}
function getWifiGW4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiGW4").value = this.responseText;
			wifiGW4Value = wifiGW4.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiGW4", true);
	xhttp.send();
}
function getWifiSN1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiSN1").value = this.responseText;
			wifiSN1Value = wifiSN1.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiSN1", true);
	xhttp.send();
}
function getWifiSN2() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiSN2").value = this.responseText;
			wifiSN2Value = wifiSN2.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiSN2", true);
	xhttp.send();
}
function getWifiSN3() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiSN3").value = this.responseText;
			wifiSN3Value = wifiSN3.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiSN3", true);
	xhttp.send();
}
function getWifiSN4() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("wifiSN4").value = this.responseText;
			wifiSN4Value = wifiSN4.valueAsNumber;
		}
	};
	xhttp.open("GET", "getWifiSN4", true);
	xhttp.send();
}

//  ****************************************************************************************************
//  EVENT LISTENER - get
//  ****************************************************************************************************
function init() {
	//  NETZWERK
	//  |-  ACCESS POINT
	getAPConfig();
	getAPSSID();
	getAPPass();
	getAPHide();
	getAPIP1();
	getAPIP2();
	getAPIP3();
	getAPIP4();
	getAPGW1();
	getAPGW2();
	getAPGW3();
	getAPGW4();
	getAPSN1();
	getAPSN2();
	getAPSN3();
	getAPSN4();
	//  |-  WIFI CLIENT
	getWifiConfig();
	getWifiSSID();
	getWifiPass();
	getWifiDHCP();
	getWifiIP1();
	getWifiIP2();
	getWifiIP3();
	getWifiIP4();
	getWifiGW1();
	getWifiGW2();
	getWifiGW3();
	getWifiGW4();
	getWifiSN1();
	getWifiSN2();
	getWifiSN3();
	getWifiSN4();
}

//  ****************************************************************************************************
//  EVENT LISTENER - upd
//  ****************************************************************************************************
function startup() {
	//  NETZWERK
	//  |-  ACCESS POINT
	const apConfig = document.querySelector("#apConfig");
	apConfig.addEventListener("input", updAPConfig, false);
	const apSSID = document.querySelector("#apSSID");
	apSSID.addEventListener("input", updAPSSID, false);
	const apPass = document.querySelector("#apPass");
	apPass.addEventListener("input", updAPPass, false);
	const apHide = document.querySelector("#apHide");
	apHide.addEventListener("input", updAPHide, false);
	const apIP1 = document.querySelector("#apIP1");
	apIP1.addEventListener("input", updAPIP1, false);
	const apIP2 = document.querySelector("#apIP2");
	apIP2.addEventListener("input", updAPIP2, false);
	const apIP3 = document.querySelector("#apIP3");
	apIP3.addEventListener("input", updAPIP3, false);
	const apIP4 = document.querySelector("#apIP4");
	apIP4.addEventListener("input", updAPIP4, false);
	const apGW1 = document.querySelector("#apGW1");
	apGW1.addEventListener("input", updAPGW1, false);
	const apGW2 = document.querySelector("#apGW2");
	apGW2.addEventListener("input", updAPGW2, false);
	const apGW3 = document.querySelector("#apGW3");
	apGW3.addEventListener("input", updAPGW3, false);
	const apGW4 = document.querySelector("#apGW4");
	apGW4.addEventListener("input", updAPGW4, false);
	const apSN1 = document.querySelector("#apSN1");
	apSN1.addEventListener("input", updAPSN1, false);
	const apSN2 = document.querySelector("#apSN2");
	apSN2.addEventListener("input", updAPSN2, false);
	const apSN3 = document.querySelector("#apSN3");
	apSN3.addEventListener("input", updAPSN3, false);
	const apSN4 = document.querySelector("#apSN4");
	apSN4.addEventListener("input", updAPSN4, false);
	const apButton = document.querySelector("#apButton");
	apButton.addEventListener("click", apRestart, false);
	//  |-  WIFI CLIENT
	const wifiConfig = document.querySelector("#wifiConfig");
	wifiConfig.addEventListener("input", updWifiConfig, false);
	const wifiSSID = document.querySelector("#wifiSSID");
	wifiSSID.addEventListener("input", updWifiSSID, false);
	const wifiPass = document.querySelector("#wifiPass");
	wifiPass.addEventListener("input", updWifiPass, false);
	const wifiDHCP = document.querySelector("#wifiDHCP");
	wifiDHCP.addEventListener("input", updWifiDHCP, false);
	const wifiIP1 = document.querySelector("#wifiIP1");
	wifiIP1.addEventListener("input", updWifiIP1, false);
	const wifiIP2 = document.querySelector("#wifiIP2");
	wifiIP2.addEventListener("input", updWifiIP2, false);
	const wifiIP3 = document.querySelector("#wifiIP3");
	wifiIP3.addEventListener("input", updWifiIP3, false);
	const wifiIP4 = document.querySelector("#wifiIP4");
	wifiIP4.addEventListener("input", updWifiIP4, false);
	const wifiGW1 = document.querySelector("#wifiGW1");
	wifiGW1.addEventListener("input", updWifiGW1, false);
	const wifiGW2 = document.querySelector("#wifiGW2");
	wifiGW2.addEventListener("input", updWifiGW2, false);
	const wifiGW3 = document.querySelector("#wifiGW3");
	wifiGW3.addEventListener("input", updWifiGW3, false);
	const wifiGW4 = document.querySelector("#wifiGW4");
	wifiGW4.addEventListener("input", updWifiGW4, false);
	const wifiSN1 = document.querySelector("#wifiSN1");
	wifiSN1.addEventListener("input", updWifiSN1, false);
	const wifiSN2 = document.querySelector("#wifiSN2");
	wifiSN2.addEventListener("input", updWifiSN2, false);
	const wifiSN3 = document.querySelector("#wifiSN3");
	wifiSN3.addEventListener("input", updWifiSN3, false);
	const wifiSN4 = document.querySelector("#wifiSN4");
	wifiSN4.addEventListener("input", updWifiSN4, false);
	const wifiButton = document.querySelector("#wifiButton");
	wifiButton.addEventListener("click", wifiRestart, false);
}

//  ****************************************************************************************************
//  FUNCTIONS - upd
//  ****************************************************************************************************
//  NETZWERK
//  |-  ACCESS POINT
function updAPConfig(event) {
	apConfigValue = event.target.checked;
	$.post("/updAPConfig", {
		apConfig: apConfigValue,
	});
	unLockAPFields();
	validatePass();
	validateIP();
	unlockAPButton();
}
function updAPSSID(event) {
	apSSIDValue = event.target.value;
	$.post("/updAPSSID", {
		apSSID: apSSIDValue,
	});
	unlockAPButton();
	unlockAPButton();
}
function updAPPass(event) {
	apPassValue = event.target.value;
	$.post("/updAPPass", {
		apPass: apPassValue,
	});
	validatePass();
	unlockAPButton();
}
function updAPHide(event) {
	apHideValue = event.target.value;
	$.post("/updAPHide", {
		apHide: apHideValue,
	});
	unlockAPButton();
}
function updAPIP1(event) {
	apIP1Value = event.target.valueAsNumber;
	$.post("/updAPIP1", {
		apIP1: apIP1Value,
	});
	apGW1.value = apIP1Value;
	apGW1Value = apIP1Value;
	validateIP();
	unlockAPButton();
}
function updAPIP2(event) {
	apIP2Value = event.target.valueAsNumber;
	$.post("/updAPIP2", {
		apIP2: apIP2Value,
	});
	apGW2.value = apIP2Value;
	apGW2Value = apIP2Value;
	validateIP();
	unlockAPButton();
}
function updAPIP3(event) {
	apIP3Value = event.target.valueAsNumber;
	$.post("/updAPIP3", {
		apIP3: apIP3Value,
	});
	apGW3.value = apIP3Value;
	apGW3Value = apIP3Value;
	validateIP();
	unlockAPButton();
}
function updAPIP4(event) {
	apIP4Value = event.target.valueAsNumber;
	$.post("/updAPIP4", {
		apIP4: apIP4Value,
	});
	validateIP();
	unlockAPButton();
}
function updAPGW1(event) {
	apGW1Value = event.target.valueAsNumber;
	$.post("/updAPGW1", {
		apGW1: apGW1Value,
	});
	unlockAPButton();
}
function updAPGW2(event) {
	apGW2Value = event.target.valueAsNumber;
	$.post("/updAPGW2", {
		apGW2: apGW2Value,
	});
	unlockAPButton();
}
function updAPGW3(event) {
	apGW3Value = event.target.valueAsNumber;
	$.post("/updAPGW3", {
		apGW3: apGW3Value,
	});
	unlockAPButton();
}
function updAPGW4(event) {
	apGW4Value = event.target.valueAsNumber;
	$.post("/updAPGW4", {
		apGW4: apGW4Value,
	});
	unlockAPButton();
}
function updAPSN1(event) {
	apSN1Value = event.target.valueAsNumber;
	$.post("/updAPSN1", {
		apSN1: apSN1Value,
	});
	unlockAPButton();
}
function updAPSN2(event) {
	apSN2Value = event.target.valueAsNumber;
	$.post("/updAPSN2", {
		apSN2: apSN2Value,
	});
	unlockAPButton();
}
function updAPSN3(event) {
	apSN3Value = event.target.valueAsNumber;
	$.post("/updAPSN3", {
		apSN3: apSN3Value,
	});
	unlockAPButton();
}
function updAPSN4(event) {
	apSN4Value = event.target.valueAsNumber;
	$.post("/updAPSN4", {
		apSN4: apSN4Value,
	});
	unlockAPButton();
}
//  |-  WIFI CLIENT
function updWifiConfig(event) {
	wifiConfigValue = event.target.checked;
	$.post("/updWifiConfig", {
		wifiConfig: wifiConfigValue,
	});
	unLockWifiFields();
	unlockWifiButton();
}
function updWifiSSID(event) {
	wifiSSIDValue = event.target.value;
	$.post("/updWifiSSID", {
		wifiSSID: wifiSSIDValue,
	});
	unlockWifiButton();
}
function updWifiPass(event) {
	wifiPassValue = event.target.value;
	$.post("/updWifiPass", {
		wifiPass: wifiPassValue,
	});
	unlockWifiButton();
}
function updWifiDHCP(event) {
	wifiDHCPValue = event.target.value;
	$.post("/updWifiDHCP", {
		wifiDHCP: wifiDHCPValue,
	});
	unlockDHCPFields();
	unlockWifiButton();
}
function updWifiIP1(event) {
	wifiIP1Value = event.target.valueAsNumber;
	$.post("/updWifiIP1", {
		wifiIP1: wifiIP1Value,
	});
	unlockWifiButton();
}
function updWifiIP2(event) {
	wifiIP2Value = event.target.valueAsNumber;
	$.post("/updWifiIP2", {
		wifiIP2: wifiIP2Value,
	});
	unlockWifiButton();
}
function updWifiIP3(event) {
	wifiIP3Value = event.target.valueAsNumber;
	$.post("/updWifiIP3", {
		wifiIP3: wifiIP3Value,
	});
	unlockWifiButton();
}
function updWifiIP4(event) {
	wifiIP4Value = event.target.valueAsNumber;
	$.post("/updWifiIP4", {
		wifiIP4: wifiIP4Value,
	});
	unlockWifiButton();
}
function updWifiGW1(event) {
	wifiGW1Value = event.target.valueAsNumber;
	$.post("/updWifiGW1", {
		wifiGW1: wifiGW1Value,
	});
	unlockWifiButton();
}
function updWifiGW2(event) {
	wifiGW2Value = event.target.valueAsNumber;
	$.post("/updWifiGW2", {
		wifiGW2: wifiGW2Value,
	});
	unlockWifiButton();
}
function updWifiGW3(event) {
	wifiGW3Value = event.target.valueAsNumber;
	$.post("/updWifiGW3", {
		wifiGW3: wifiGW3Value,
	});
	unlockWifiButton();
}
function updWifiGW4(event) {
	wifiGW4Value = event.target.valueAsNumber;
	$.post("/updWifiGW4", {
		wifiGW4: wifiGW4Value,
	});
	unlockWifiButton();
}
function updWifiSN1(event) {
	wifiSN1Value = event.target.valueAsNumber;
	$.post("/updWifiSN1", {
		wifiSN1: wifiSN1Value,
	});
	unlockWifiButton();
}
function updWifiSN2(event) {
	wifiSN2Value = event.target.valueAsNumber;
	$.post("/updWifiSN2", {
		wifiSN2: wifiSN2Value,
	});
	unlockWifiButton();
}
function updWifiSN3(event) {
	wifiSN3Value = event.target.valueAsNumber;
	$.post("/updWifiSN3", {
		wifiSN3: wifiSN3Value,
	});
	unlockWifiButton();
}
function updWifiSN4(event) {
	wifiSN4Value = event.target.valueAsNumber;
	$.post("/updWifiSN4", {
		wifiSN4: wifiSN4Value,
	});
	unlockWifiButton();
}
//  |-  LOCK/UNLOCK FIELDS
function unLockAPFields() {
	if (apConfigValue === true) {
		apSSID.disabled = false;
		apPass.disabled = false;
		apHide.disabled = false;
		apIP1.disabled = false;
		apIP2.disabled = false;
		apIP3.disabled = false;
		apIP4.disabled = false;
		apGW1.disabled = false;
		apGW2.disabled = false;
		apGW3.disabled = false;
		apGW4.disabled = false;
		apSN1.disabled = false;
		apSN2.disabled = false;
		apSN3.disabled = false;
		apSN4.disabled = false;
	}
	if (apConfigValue === false) {
		apSSID.disabled = true;
		apPass.disabled = true;
		apHide.disabled = true;
		apIP1.disabled = true;
		apIP2.disabled = true;
		apIP3.disabled = true;
		apIP4.disabled = true;
		apGW1.disabled = true;
		apGW2.disabled = true;
		apGW3.disabled = true;
		apGW4.disabled = true;
		apSN1.disabled = true;
		apSN2.disabled = true;
		apSN3.disabled = true;
		apSN4.disabled = true;
	}
}
function unLockWifiFields() {
	if (wifiConfigValue === true) {
		wifiSSID.disabled = false;
		wifiPass.disabled = false;
		wifiDHCP.disabled = false;
	}
	if (wifiConfigValue === true && wifiDHCPValue === "1") {
		wifiIP1.disabled = false;
		wifiIP2.disabled = false;
		wifiIP3.disabled = false;
		wifiIP4.disabled = false;
		wifiGW1.disabled = false;
		wifiGW2.disabled = false;
		wifiGW3.disabled = false;
		wifiGW4.disabled = false;
		wifiSN1.disabled = false;
		wifiSN2.disabled = false;
		wifiSN3.disabled = false;
		wifiSN4.disabled = false;
	}
	if (wifiConfigValue === false) {
		wifiSSID.disabled = true;
		wifiPass.disabled = true;
		wifiDHCP.disabled = true;
	}
	if (wifiConfigValue === false && wifiDHCPValue === "1") {
		wifiIP1.disabled = true;
		wifiIP2.disabled = true;
		wifiIP3.disabled = true;
		wifiIP4.disabled = true;
		wifiGW1.disabled = true;
		wifiGW2.disabled = true;
		wifiGW3.disabled = true;
		wifiGW4.disabled = true;
		wifiSN1.disabled = true;
		wifiSN2.disabled = true;
		wifiSN3.disabled = true;
		wifiSN4.disabled = true;
	}
}
function unlockDHCPFields() {
	if (wifiDHCPValue === "0") {
		wifiIP1.disabled = true;
		wifiIP2.disabled = true;
		wifiIP3.disabled = true;
		wifiIP4.disabled = true;
		wifiGW1.disabled = true;
		wifiGW2.disabled = true;
		wifiGW3.disabled = true;
		wifiGW4.disabled = true;
		wifiSN1.disabled = true;
		wifiSN2.disabled = true;
		wifiSN3.disabled = true;
		wifiSN4.disabled = true;
	}
	if (wifiDHCPValue === "1") {
		wifiIP1.disabled = false;
		wifiIP2.disabled = false;
		wifiIP3.disabled = false;
		wifiIP4.disabled = false;
		wifiGW1.disabled = false;
		wifiGW2.disabled = false;
		wifiGW3.disabled = false;
		wifiGW4.disabled = false;
		wifiSN1.disabled = false;
		wifiSN2.disabled = false;
		wifiSN3.disabled = false;
		wifiSN4.disabled = false;
	}
}
//  |-  VALIDATE FIELDS
function validatePass() {
	if (apConfigValue === true) {
		if (
			apPassValue.length >= 8 &&
			apPassValue.length <= 63 &&
			apPassValue.match(/[a-z]/) &&
			apPassValue.match(/[A-Z]/) &&
			apPassValue.match(/\d/) &&
			apPassValue.match(/[^a-zA-Z\d]/)
		) {
			apPass.classList.remove("is-invalid");
			apPassError = 0;
		} else {
			apPass.classList.add("is-invalid");
			apPassError = 1;
		}
	} else {
		apPass.classList.remove("is-invalid");
		apPassError = 0;
	}
}
function validateIP() {
	if (apConfigValue === true) {
		if (apIP1Value !== "" && (apIP1Value === 10 || apIP1Value === 172 || apIP1Value === 192)) {
			apIP1.classList.remove("is-invalid");
			apIP1Error = 0;
		} else {
			apIP1.classList.add("is-invalid");
			apIP1Error = 1;
		}
		if (
			apIP2Value !== "" &&
			((apIP1Value === 10 && apIP2Value >= 0 && apIP2Value <= 255) ||
				(apIP1Value === 172 && apIP2Value >= 16 && apIP2Value <= 31) ||
				(apIP1Value === 192 && apIP2Value === 168))
		) {
			apIP2.classList.remove("is-invalid");
			apIP2Error = 0;
		} else {
			apIP2.classList.add("is-invalid");
			apIP2Error = 1;
		}
		if (apIP3Value !== "" && apIP3Value >= 0 && apIP3Value <= 255) {
			apIP3.classList.remove("is-invalid");
			apIP3Error = 0;
		} else {
			apIP3.classList.add("is-invalid");
			apIP3Error = 1;
		}
		if (apIP4Value !== "" && apIP4Value >= 1 && apIP4Value <= 254) {
			apIP4.classList.remove("is-invalid");
			apIP4Error = 0;
		} else {
			apIP4.classList.add("is-invalid");
			apIP4Error = 1;
		}
	} else {
		apIP1.classList.remove("is-invalid");
		apIP1Error = 0;
		apIP2.classList.remove("is-invalid");
		apIP2Error = 0;
		apIP3.classList.remove("is-invalid");
		apIP3Error = 0;
		apIP4.classList.remove("is-invalid");
		apIP4Error = 0;
	}
}
//  |-  BUTTONS
function unlockAPButton() {
	apButton.classList.remove("btn-outline-secondary");
	apButton.classList.add("btn-outline-danger");
	if (apPassError === 0 && apIP1Error === 0 && apIP2Error === 0 && apIP3Error === 0 && apIP4Error === 0) {
		apButton.classList.remove("btn-outline-danger");
		apButton.classList.add("btn-danger");
		apButton.disabled = false;
	} else {
		apButton.classList.remove("btn-danger");
		apButton.classList.add("btn-outline-danger");
		apButton.disabled = true;
	}
}
function apRestart() {
	if (apPassError === 0 && apIP1Error === 0 && apIP2Error === 0 && apIP3Error === 0 && apIP4Error === 0) {
		$.post("/apRestart");
	}
}
function unlockWifiButton() {
	wifiButton.classList.remove("btn-outline-secondary");
	wifiButton.classList.add("btn-danger");
	wifiButton.disabled = false;
}
function wifiRestart() {
	$.post("/wifiRestart");
	resetWifiButton();
}
function resetWifiButton() {
	wifiButton.classList.remove("btn-danger");
	wifiButton.classList.add("btn-outline-secondary");
	wifiButton.disabled = true;
}
