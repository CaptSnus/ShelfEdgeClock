//  Date & Time
$(document).ready(function () {
	if ($("#currDT").length) {
		window.setInterval(getDT, 900);
	}
});

function getDT() {
	document.getElementById("currDT").value = moment().format("DD. MMMM YYYY - HH:mm:ss");
}

function updDT() {
	const y = moment().format("YYYY");
	const n = moment().format("MM");
	const d = moment().format("DD");
	const h = moment().format("HH");
	const m = moment().format("mm");
	const s = moment().format("ss");
	const datetime = { year: y, month: n, day: d, hour: h, min: m, sec: s };
	$.post("/updDateTime", datetime);
}

// resets
function resetFlash() {
	$.post("/resetFlash");
}

function resetESP() {
	$.post("/resetESP");
}
