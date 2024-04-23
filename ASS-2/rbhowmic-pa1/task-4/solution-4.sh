# Get the list of infloop processes created by the current user
processList=$(ps -u $(whoami) -o pid,cmd | grep 'infloop' | awk '{print $1}')
# Check if there are any infloop processes
if [ -n "$processList" ]; then
  echo "Killing infloop processes..."
  echo "$processList" | xargs kill -9
  echo "infloop processes killed."
else
  echo "No infloop processes found."
fi

