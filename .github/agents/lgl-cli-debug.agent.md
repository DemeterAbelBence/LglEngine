---
description: This custom agent handles CLI and debug functionalities for the LglEngine project.
model: GPT-4.1
tools: [execute, read, edit, search, web, agent, todo]
handoffs:
  - label: Start Implementation
    agent: agent
    prompt: Implement the plan
    send: true
    model: GPT-4.1 (copilot)
---