//  ****************************************************************************************************
//  FUNCTIONS - update
//  ****************************************************************************************************

//  Mode
function updateDisplayMode(mode, data1 = 0, data2 = 0) {
	const displayMode = { mode: mode, data1: data1, data2: data2 };
	$.post("/updateDisplayMode", displayMode);
	console.log(displayMode);
}

//  Scoreboard
function scoreLup() {
	const scoreL = $("#scoreL").val();
	if (scoreL < 99 && !isNaN(scoreL)) {
		const newval = parseInt(scoreL) + 1;
		$("#scoreL").val(newval);
		scoreShow();
	}
}
function scoreLdown() {
	const scoreL = $("#scoreL").val();
	if (scoreL > 0 && !isNaN(scoreL)) {
		const newval = parseInt(scoreL) - 1;
		$("#scoreL").val(newval);
		scoreShow();
	}
}
function scoreRup() {
	const scoreR = $("#scoreR").val();
	if (scoreR < 99 && !isNaN(scoreR)) {
		const newval = parseInt(scoreR) + 1;
		$("#scoreR").val(newval);
		scoreShow();
	}
}
function scoreRdown() {
	const scoreR = $("#scoreR").val();
	if (scoreR > 0 && !isNaN(scoreR)) {
		const newval = parseInt(scoreR) - 1;
		$("#scoreR").val(newval);
		scoreShow();
	}
}
function scoreShow() {
	const scoreL = parseInt($("#scoreL").val());
	const scoreR = parseInt($("#scoreR").val());
	updateDisplayMode(4, scoreL, scoreR);
}
function scoreReset() {
	$("#scoreL").val(0);
	$("#scoreR").val(0);
	scoreShow();
}

//  Countdown
function countHup() {
	const countH = $("#countH").val();
	if (countH < 23 && !isNaN(countH)) {
		const newval = parseInt(countH) + 1;
		$("#countH").val(newval);
	}
}
function countMup() {
	const countM = $("#countM").val();
	if (countM < 60 && !isNaN(countM)) {
		const newval = parseInt(countM) + 1;
		$("#countM").val(newval);
	}
}
function countSup() {
	const countS = $("#countS").val();
	if (countS < 60 && !isNaN(countS)) {
		const newval = parseInt(countS) + 1;
		$("#countS").val(newval);
	}
}
function countHdown() {
	const countH = $("#countH").val();
	if (countH > 0 && !isNaN(countH)) {
		const newval = parseInt(countH) - 1;
		$("#countH").val(newval);
	}
}
function countMdown() {
	const countM = $("#countM").val();
	if (countM > 0 && !isNaN(countM)) {
		const newval = parseInt(countM) - 1;
		$("#countM").val(newval);
	}
}
function countSdown() {
	const countS = $("#countS").val();
	if (countS > 0 && !isNaN(countS)) {
		const newval = parseInt(countS) - 1;
		$("#countS").val(newval);
	}
}
function countStart() {
	const h = $("#countH").val();
	const m = $("#countM").val();
	const s = $("#countS").val();
	const ms = h * 60 * 60 * 1000 + m * 60 * 1000 + s * 1000;
	updateDisplayMode(5, ms);
}
function countReset() {
	$("#countH").val(0);
	$("#countM").val(0);
	$("#countS").val(0);
	countStart();
}

//  Downlights
function updateDownlight(downlightUsage, downlightColor) {
	const downlight = { downlightUsage: downlightUsage, downlightColor: downlightColor };
	$.post("/updateDownlight", downlight);
	console.log(downlight);
}
