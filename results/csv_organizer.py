import pandas as pd
from pathlib import Path

# ✅ Use raw string for Windows paths to avoid issues with backslashes
data_dir = Path(r"C:\Users\Håkon Paulsrud\Desktop\results")  # <-- Make sure this folder exists
output_file = data_dir / "combined_tables.xlsx"

# Rename mapping
rename_map = {
    "Text1": "Reference",
    "Text2": "AI_comments",
    "Text3": "AI_nocomments"
}

all_tables = []

# Process all CSV files in folder
for file in sorted(data_dir.glob("*.csv")):
    df = pd.read_csv(file)

    # Rename both rows and columns
    df.rename(columns=rename_map, inplace=True)
    df.rename(index=dict(zip(df['Unnamed: 0'], df['Unnamed: 0'].map(rename_map))), inplace=True)
    df.drop(columns=["Unnamed: 0"], inplace=True)

    # Round to 3 decimals
    df = df.round(3)

    # Add filename header
    header = pd.DataFrame([[file.stem] + [''] * (df.shape[1] - 1)], columns=df.columns)
    
    # Append to master list
    all_tables.extend([header, df, pd.DataFrame([[''] * df.shape[1]], columns=df.columns)])

# Combine all
final_df = pd.concat(all_tables, ignore_index=True)

# Save as Excel
final_df.to_excel(output_file, index=False)
print(f"Combined Excel saved to: {output_file}")
