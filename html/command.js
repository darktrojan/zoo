var commands = document.querySelectorAll('.command');
var current_command = -1;

function start_commands() {
	document.getElementById('beforecommands').classList.add('hidden');
	run_next_command();
}

function run_command(aCommandElement, aCallback) {
	var data = 'command=' + aCommandElement.dataset.name;
	if (aCommandElement.dataset.params) {
		data += '&' + aCommandElement.dataset.params;
	}
	aCommandElement.classList.add('running');
	aCommandElement.classList.remove('hidden');
	XHR.post('', data, function(aRequest) {
		aCommandElement.classList.remove('running');
		var pre = document.createElement('pre');
		pre.textContent = aRequest.responseText;
		aCommandElement.appendChild(pre);
		aCallback(aRequest);
	});
}

function run_next_command() {
	current_command++;
	if (current_command < commands.length) {
		run_command(commands[current_command], run_next_command);
		return;
	}

	var commands_element = document.getElementById('commands');
	if (commands_element.dataset.redirect) {
		location.replace(commands_element.dataset.redirect);
	} else {
		document.getElementById('aftercommands').classList.remove('hidden');
	}
}
