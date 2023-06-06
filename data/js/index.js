//  INDEX.HTML

//  Mode
function goClk() {
	$.post("/goModeClk");
	console.log("clock");
}
function goDat() {
	$.post("/goModeDat");
	console.log("date");
}
function goTem() {
	$.post("/goModeTem");
	console.log("temperature");
}
function goHum() {
	$.post("/goModeHum");
	console.log("humidity");
}
function goScr() {
	$.post("/goModeScr");
	console.log("scroll");
}


//  Scoreboard
function startup() {
	const scoHValue = document.querySelector("#scoHome");
	scoHValue.addEventListener("input", scoShow, false);
	const scoAValue = document.querySelector("#scoAway");
	scoAValue.addEventListener("input", scoShow, false);
}

function scoHUp() {
	const scoHome = $("#scoHome").val();
	if (scoHome < 99 && !isNaN(scoHome)) {
		const newval = parseInt(scoHome) + 1;
		$("#scoHome").val(newval);
		scoShow();
	}
}

function scoAUp() {
	const scoAway = $("#scoAway").val();
	if (scoAway < 99 && !isNaN(scoAway)) {
		const newval = parseInt(scoAway) + 1;
		$("#scoAway").val(newval);
		scoShow();
	}
}

function scoHDown() {
	const scoHome = $("#scoHome").val();
	if (scoHome > 0 && !isNaN(scoHome)) {
		const newval = parseInt(scoHome) - 1;
		$("#scoHome").val(newval);
		scoShow();
	}
}

function scoADown() {
	const scoAway = $("#scoAway").val();
	if (scoAway > 0 && !isNaN(scoAway)) {
		const newval = parseInt(scoAway) - 1;
		$("#scoAway").val(newval);
		scoShow();
	}
}

function scoShow() {
	const scoH = $("#scoHome").val();
	const scoA = $("#scoAway").val();
	const scoreboard = { scoHome: scoH, scoAway: scoA };
	$.post("/goModeSco", scoreboard);
	console.log(scoreboard);
}

function scoReset() {
	$("#scoHome").val(0);
	$("#scoAway").val(0);
	scoShow();
}


//  Countdown
function couHUp() {
	const couH = $("#couH").val();
	if (couH < 99 && !isNaN(couH)) {
		const newval = parseInt(couH) + 1;
		$("#couH").val(newval);
	}
}

function couMUp() {
	const couM = $("#couM").val();
	if (couM < 60 && !isNaN(couM)) {
		const newval = parseInt(couM) + 1;
		$("#couM").val(newval);
	}
}

function couSUp() {
	const couS = $("#couS").val();
	if (couS < 60 && !isNaN(couS)) {
		const newval = parseInt(couS) + 1;
		$("#couS").val(newval);
	}
}

function couHDown() {
	const couH = $("#couH").val();
	if (couH > 0 && !isNaN(couH)) {
		const newval = parseInt(couH) - 1;
		$("#couH").val(newval);
	}
}

function couMDown() {
	const couM = $("#couM").val();
	if (couM > 0 && !isNaN(couM)) {
		const newval = parseInt(couM) - 1;
		$("#couM").val(newval);
	}
}

function couSDown() {
	const couS = $("#couS").val();
	if (couS > 0 && !isNaN(couS)) {
		const newval = parseInt(couS) - 1;
		$("#couS").val(newval);
	}
}

function couStart() {
	const h = $("#couH").val();
	const m = $("#couM").val();
	const s = $("#couS").val();
	const ms = h * 60 * 60 * 1000 + m * 60 * 1000 + s * 1000;
	const countdown = { millis: ms };
	$.post("/goModeCou", countdown);
}

function couReset() {
	$("#couH").val(0);
	$("#couM").val(0);
	$("#couS").val(0);
	couStart();
}


//  Downlights
function goDow(dowU, dowCS) {
	const downlight = { dowUsage: dowU, dowColorSet: dowCS };
	$.post("/goDow", downlight);
	console.log(downlight);
}
