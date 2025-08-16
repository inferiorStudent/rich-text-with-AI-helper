import torch
from fastapi import FastAPI
from pydantic import BaseModel
from transformers import AutoTokenizer, AutoModelForCausalLM
import uvicorn

device = 'cuda' if torch.cuda.is_available() else 'cpu'
model_name = "bigscience/bloomz-560m"
# model_name = 'Qwen/Qwen3-0.6B'
mirror_path = "https://mirrors.tuna.tsinghua.edu.cn/hugging-face/"
store_path = "./model"
# store_path = './qwen'

tokenizer = AutoTokenizer.from_pretrained(store_path)
model = AutoModelForCausalLM.from_pretrained(
    store_path,
    torch_dtype=torch.float16
)
model = model.to(device)

app = FastAPI()

class TextInput(BaseModel):
    text: str

class SuggestionOutput(BaseModel):
    suggestion: str

@app.post('/generate', response_model=SuggestionOutput)
async def generate_suggestion(text_input: TextInput):
    '''
    :param text_input:  接收用户输入文本
    :return:            返回补全建议
    '''
    prompt = text_input.text
    if not prompt:
        return {'suggestion': '<UNK>'}
    try:
        inputs = tokenizer(prompt, return_tensors="pt").to(device)
        output_sequences = model.generate(
            input_ids=inputs["input_ids"],
            attention_mask=inputs["attention_mask"],
            max_new_tokens=100,
            num_return_sequences=1,
            no_repeat_ngram_size=3,
            pad_token_id=tokenizer.eos_token_id,
            eos_token_id=tokenizer.eos_token_id
        )

        full_text = tokenizer.decode(output_sequences[0], skip_special_tokens=True)
        suggestion = full_text[len(prompt):]
        return {'suggestion': suggestion}
    except Exception as e:
        return {'suggestion': '<UNK>'}

# http://127.0.0.1:8000/generate

if __name__ == '__main__':
    print(device)
    uvicorn.run(app, host='127.0.0.1', port=8000)