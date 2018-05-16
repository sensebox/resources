#!/bin/bash
set -euo pipefail

# Get the latest tags on master
latest_tag=$(git describe --tags $(git rev-list --tags --max-count=1))
previous_tag=$(git describe --tags $(git rev-list --tags --skip=1 --max-count=1))

# echo markdown with tag name to changelog
echo "## $latest_tag" >> CHANGELOG.md

# Get all commit messages between latest tags and append them to changelog
git log --pretty=format:"Subject: %s (%cn)" $previous_tag...$latest_tag >> CHANGELOG.md