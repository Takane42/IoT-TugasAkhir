// Update Chart.js configuration to ensure proper centering and responsiveness
Chart.defaults.responsive = true;
Chart.defaults.maintainAspectRatio = false;

// Update chart configurations to match the reference design
document.addEventListener('DOMContentLoaded', function() {
    // Override the chart configurations from app.js
    if (window.suhuChart) {
        suhuChart.options.aspectRatio = 2.5;
        suhuChart.options.plugins = {
            legend: {
                position: 'top',
                align: 'start'
            }
        };
        suhuChart.update();
    }
    
    if (window.kelembabanChart) {
        kelembabanChart.options.aspectRatio = 2.5;
        kelembabanChart.options.plugins = {
            legend: {
                position: 'top',
                align: 'start'
            }
        };
        kelembabanChart.update();
    }
});

// Function to update the UI when connection status changes
function updateStatusUI(status, color) {
    const statusSpan = document.getElementById('status');
    statusSpan.textContent = status;
    statusSpan.className = '';
    statusSpan.classList.add('fw-bold', `text-${color}`);
}

// Override the status update in the original app.js
const originalOnConnect = client.onConnect;
client.onConnect = function() {
    if (originalOnConnect) originalOnConnect();
    updateStatusUI('Connected', 'success');
};

const originalOnFailure = client.onFailure;
client.onFailure = function(response) {
    if (originalOnFailure) originalOnFailure(response);
    updateStatusUI(`Connection Failed: ${response.errorMessage}`, 'danger');
};

const originalOnConnectionLost = client.onConnectionLost;
client.onConnectionLost = function(responseObject) {
    if (originalOnConnectionLost) originalOnConnectionLost(responseObject);
    if (responseObject.errorCode !== 0) {
        updateStatusUI(`Connection Lost: ${responseObject.errorMessage}`, 'danger');
    }
};

// Function to update switch UI
function updateSwitchUI(pin, isOn) {
    const label = document.getElementById(`label-${pin}`);
    const checkbox = document.getElementById(`switch-${pin}`);
    
    if (isOn) {
        label.textContent = 'ON';
        label.className = 'badge bg-success mt-2';
        checkbox.checked = true;
    } else {
        label.textContent = 'OFF';
        label.className = 'badge bg-secondary mt-2';
        checkbox.checked = false;
    }
}
