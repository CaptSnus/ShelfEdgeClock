//  ****************************************************************************************************
//  VARIABLES
//  ****************************************************************************************************
//  access point
let apSwitchValue;
let apSSIDValue;
let apSSIDError;
let apPassValue;
let apPassError;
let apHiddenValue;
let apIP1Value;
let apIP1Error;
let apIP2Value;
let apIP2Error;
let apIP3Value;
let apIP3Error;

//  wifi client
let wifiSwitchValue;
let wifiSSIDValue;
let wifiPassValue;

//  ****************************************************************************************************
//  FUNCTIONS - get
//  ****************************************************************************************************
//  access point
function getAPSwitch() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("apSwitch").checked = false;
				apSwitchValue = false;
				toggleAPfields();
			}
			if (this.responseText === "1") {
				document.getElementById("apSwitch").checked = true;
				apSwitchValue = true;
				toggleAPfields();
			}
		}
	};
	xhttp.open("GET", "getAPSwitch", true);
	xhttp.send();
}
function getAPSSID() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apSSID").value = this.responseText;
			apSSIDValue = apSSID.value;
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
		}
	};
	xhttp.open("GET", "getAPPass", true);
	xhttp.send();
}
function getAPHidden() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apHidden").value = this.responseText;
		}
	};
	xhttp.open("GET", "getAPHidden", true);
	xhttp.send();
}
function getAPIP1() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			document.getElementById("apIP1").value = this.responseText;
			apIP1Value = apIP1.valueAsNumber;
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
		}
	};
	xhttp.open("GET", "getAPIP3", true);
	xhttp.send();
}
//  wifi client
function getWifiSwitch() {
	const xhttp = new XMLHttpRequest();
	xhttp.onreadystatechange = function () {
		if (this.readyState === 4 && this.status === 200) {
			if (this.responseText === "0") {
				document.getElementById("wifiSwitch").checked = false;
				wifiSwitchValue = false;
				toggleWIFIfields();
			}
			if (this.responseText === "1") {
				document.getElementById("wifiSwitch").checked = true;
				wifiSwitchValue = true;
				toggleWIFIfields();
			}
		}
	};
	xhttp.open("GET", "getWifiSwitch", true);
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

//  ****************************************************************************************************
//  LOAD VALUES - get
//  ****************************************************************************************************
function init() {
	//  access point
	getAPSwitch();
	getAPSSID();
	getAPPass();
	getAPHidden();
	getAPIP1();
	getAPIP2();
	getAPIP3();

	//  wifi client
	getWifiSwitch();
	getWifiSSID();
	getWifiPass();
}

//  ****************************************************************************************************
//  EVENT LISTENER - update
//  ****************************************************************************************************
function startup() {
	//  access point
	const apSwitch = document.querySelector("#apSwitch");
	apSwitch.addEventListener("input", updAPSwitch, false);
	const apSSID = document.querySelector("#apSSID");
	apSSID.addEventListener("input", updAPSSID, false);
	const apPass = document.querySelector("#apPass");
	apPass.addEventListener("input", updAPPassword, false);
	const apHidden = document.querySelector("#apHidden");
	apHidden.addEventListener("input", updAPHidden, false);
	const apIP1 = document.querySelector("#apIP1");
	apIP1.addEventListener("input", updAPIP1, false);
	const apIP2 = document.querySelector("#apIP2");
	apIP2.addEventListener("input", updAPIP2, false);
	const apIP3 = document.querySelector("#apIP3");
	apIP3.addEventListener("input", updAPIP3, false);

	//  wifi client
	const wifiSwitch = document.querySelector("#wifiSwitch");
	wifiSwitch.addEventListener("input", updWifiSwitch, false);
	const wifiSSID = document.querySelector("#wifiSSID");
	wifiSSID.addEventListener("input", updWifiSSID, false);
	const wifiPass = document.querySelector("#wifiPass");
	wifiPass.addEventListener("input", updWifiPass, false);

	// button
	const wifiButton = document.querySelector("#wifiButton");
	wifiButton.addEventListener("click", updWifiRestart, false);
}

//  ****************************************************************************************************
//  FUNCTIONS - update
//  ****************************************************************************************************
//  access point
function updAPSwitch(event) {
	apSwitchValue = event.target.checked;
	$.post("/updAPSwitch", { apSwitch: apSwitchValue });
	console.log(apSwitchValue);
	toggleAPfields();
	toggleWifiRestart();
}
function updAPSSID(event) {
	apSSIDValue = event.target.value;
	$.post("/updAPSSID", { apSSID: apSSIDValue });
	console.log(apSSIDValue);
	validateAPSSID();
	toggleWifiRestart();
}
function updAPPassword(event) {
	apPassValue = event.target.value;
	$.post("/updAPPass", { apPass: apPassValue });
	console.log(apPassValue);
	validateAPPassword();
	toggleWifiRestart();
}
function updAPHidden(event) {
	apHiddenValue = event.target.value;
	$.post("/updAPHidden", { apHidden: apHiddenValue });
	console.log(apHiddenValue);
	toggleWifiRestart();
}
function updAPIP1(event) {
	apIP1Value = event.target.valueAsNumber;
	$.post("/updAPIP1", { apIP1: apIP1Value });
	console.log(apIP1Value);
	validateAPIP1();
	toggleWifiRestart();
}
function updAPIP2(event) {
	apIP2Value = event.target.valueAsNumber;
	$.post("/updAPIP2", { apIP2: apIP2Value });
	console.log(apIP2Value);
	validateAPIP2();
	toggleWifiRestart();
}
function updAPIP3(event) {
	apIP3Value = event.target.valueAsNumber;
	$.post("/updAPIP3", { apIP3: apIP3Value });
	console.log(apIP3Value);
	validateAPIP3();
	toggleWifiRestart();
}
//  wifi client
function updWifiSwitch(event) {
	wifiSwitchValue = event.target.checked;
	$.post("/updWifiSwitch", { wifiSwitch: wifiSwitchValue });
	console.log(wifiSwitchValue);
	toggleWIFIfields();
	toggleWifiRestart();
}
function updWifiSSID(event) {
	wifiSSIDValue = event.target.value;
	$.post("/updWifiSSID", { wifiSSID: wifiSSIDValue });
	console.log(wifiSSIDValue);
	toggleWifiRestart();
}
function updWifiPass(event) {
	wifiPassValue = event.target.value;
	$.post("/updWifiPass", { wifiPass: wifiPassValue });
	console.log(wifiPassValue);
	toggleWifiRestart();
}

//  ****************************************************************************************************
//  FUNCTIONS - support
//  ****************************************************************************************************
function toggleAPfields() {
	if (apSwitchValue === true) {
		apSSID.disabled = false;
		apPass.disabled = false;
		apHidden.disabled = false;
		apIP1.disabled = false;
		apIP2.disabled = false;
		apIP3.disabled = false;
		apIP4.disabled = false;
		apIP4.value = 1;
	}
	if (apSwitchValue === false) {
		apSSID.disabled = true;
		apPass.disabled = true;
		apHidden.disabled = true;
		apIP1.disabled = true;
		apIP2.disabled = true;
		apIP3.disabled = true;
		apIP4.disabled = true;
	}
}
function toggleWIFIfields() {
	if (wifiSwitchValue === true) {
		wifiSSID.disabled = false;
		wifiPass.disabled = false;
	}
	if (wifiSwitchValue === false) {
		wifiSSID.disabled = true;
		wifiPass.disabled = true;
	}
}
function toggleWifiRestart() {
	if (apSwitchValue === true) {
		validateAPSSID();
		validateAPPassword();
		validateAPIP1();
		validateAPIP2();
		validateAPIP3();
	}
	wifiButton.classList.remove("btn-outline-secondary");
	wifiButton.classList.add("btn-outline-danger");
	if (
		apSwitchValue === false ||
		(apSwitchValue === true &&
			apSSIDError === 0 &&
			apPassError === 0 &&
			apIP1Error === 0 &&
			apIP2Error === 0 &&
			apIP3Error === 0)
	) {
		wifiButton.classList.remove("btn-outline-danger");
		wifiButton.classList.add("btn-danger");
		wifiButton.disabled = false;
	} else {
		wifiButton.classList.add("btn-outline-danger");
		wifiButton.classList.remove("btn-danger");
		wifiButton.disabled = true;
	}
}
function updWifiRestart() {
	console.log("restart wifi");
	$.post("/updWifiRestart");
	wifiButton.classList.remove("btn-danger");
	wifiButton.classList.add("btn-outline-secondary");
	wifiButton.disabled = true;
}

//  validate fields
function validateAPSSID() {
	if (apSwitchValue === true) {
		if (apSSIDValue.length <= 32) {
			apSSID.classList.remove("is-invalid");
			apSSIDError = 0;
		} else {
			apSSID.classList.add("is-invalid");
			apSSIDError = 1;
		}
	} else {
		apSSID.classList.remove("is-invalid");
		apSSIDError = 0;
	}
}
function validateAPPassword() {
	if (apSwitchValue === true) {
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
function validateAPIP1() {
	if (apSwitchValue === true) {
		if (apIP1Value !== "" && (apIP1Value === 10 || apIP1Value === 172 || apIP1Value === 192)) {
			apIP1.classList.remove("is-invalid");
			apIP1Error = 0;
		} else {
			apIP1.classList.add("is-invalid");
			apIP1Error = 1;
		}
	}
}
function validateAPIP2() {
	if (apSwitchValue === true) {
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
	}
}
function validateAPIP3() {
	if (apSwitchValue === true) {
		if (apIP3Value !== "" && apIP3Value >= 0 && apIP3Value <= 255) {
			apIP3.classList.remove("is-invalid");
			apIP3Error = 0;
		} else {
			apIP3.classList.add("is-invalid");
			apIP3Error = 1;
		}
	}
}
