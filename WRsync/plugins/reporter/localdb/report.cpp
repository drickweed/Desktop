#include "report.h"

Result Report::toResult() const {
	Result result(source());
	result.setExitCode(exitCode());
	result.setState((Result::State) state());
	result.setStartedAt(startedAt());
	result.setDestination(EPath(destinationPath()));
	return result;
}

void Report::fromResult(Result result) {
	setExitCode(result.exitCode());
	setSource(result.sourcePath());
	setStartedAt(result.startedAt());
	setState(result.state());
	setDestinationPath(result.destination().realPath());
	if (result.destination().isLocalPath())
		setDestinationId(result.destination().driveID());
}
