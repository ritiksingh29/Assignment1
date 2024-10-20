document.getElementById("ruleForm").addEventListener("submit", async (e) => {
  e.preventDefault();
  const rule = document.getElementById("rule").value;

  const response = await fetch("/api/create_rule", {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ rule }),
  });
  const result = await response.json();
  document.getElementById("createResult").innerText = result.message;
});

document
  .getElementById("evaluateForm")
  .addEventListener("submit", async (e) => {
    e.preventDefault();
    const age = document.getElementById("age").value;
    const salary = document.getElementById("salary").value;
    const department = document.getElementById("department").value;

    const response = await fetch("/api/evaluate_rule", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        user_data: { age, salary, department },
      }),
    });
    const result = await response.json();
    document.getElementById("evaluateResult").innerText =
      "Evaluation Result: " + result.result;
  });
