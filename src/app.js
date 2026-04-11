// Copyright Titanium I.T. LLC.
import * as rot13 from "./logic/rot13.js";
import { CommandLine } from "./infrastructure/command_line.js";

export class App {

	static create() {
		return new App(CommandLine.create());
	}

	constructor(commandLine) {
		this._commandLine = commandLine;
	}

	run() {
    const args = this._commandLine.args();
    //how the app execute those command line?
    //how the app interact/return output from commandline?
    if (args.length === 0) {
			this._commandLine.writeOutput("Usage: run text_to_transform\n");
      //commandLineOutput.write();
			return;
		}
		if (args.length !== 1) {
			this._commandLine.writeOutput("too many arguments\n");
			return;
		}

		const input = args[0];
		const output = rot13.transform(input);
		this._commandLine.writeOutput(output + "\n");
	}

}

// arg1 arg2
// ./app.js arg1 arg2